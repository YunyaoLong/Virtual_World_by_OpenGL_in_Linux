/************************************************************
* writer : Yao Yunlong, Sun Yat-Sen University, SYSU        *
* date : 15/7/2017                                          *
* If you found this code useful, please let me know.        *
*                                                           *
* url : http://yaoyl.cn or http://my.csdn.net/Michael753951 *
* github : https://github.com/LongyunYao                    *
* (email me at yaoyl753951@gmail.com)                       *
*************************************************************/
//
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/freeglut.h>
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep.
#include "tcp_server.h"
#include "image.h"
#include "texture.h"
#include "Objects.h"
#include "config.h"
#include "TimeTool.h"
#include "WorldMap.h"
#include "Character.h"  // 获取主角的各个参数

/* ascii code for the escape key */
#define ESCAPE 27
#define CHANGE_MODE 99
#define DISAPPEAR_BOX 100
#define CHANGE_TEXTURE 116

/* The number of our GLUT window */
int window;
// disappear 用来存放障碍物将要消失的时间
int disappearTime;

// 记录当前时间和一些参数
FILE *fp = NULL;

WorldMap worldMap;
Config config;
Character *character;

#include <pthread.h>

/// TCP
#define HELLO_WORLD_SERVER_PORT    4000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
struct sockaddr_in server_addr, client_addr;
int server_socket, client_socket;
char each_line_data[BUFFER_SIZE];


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height) {        // We call this right after our OpenGL window is created.
    loadGLTextures();				// 装载纹理
    BuildList();                                // set up our display lists.
    //glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping 绘图前打开, 绘制结束以后关闭
    glEnable(GL_BLEND);

    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);	// Clear The Background Color To Blue
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);			// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);

    srand(time(NULL));
    disappearTime = 7;

    fp = fopen("rate_recorder.txt", "a+"); // a+意味着在文本最后追加
    fp = fopen("recorder.txt", "a+"); // a+意味着在文本最后追加

    config.initConfig("conf/sysProperties.cfg");
    worldMap.initWorldMap("Data/WorldMap.txt");

    // 读取配置文件
    GLfloat initX = getFloatVal(config.getConfig("positionX")),
        initY = getFloatVal(config.getConfig("positionY")),
        initZ = getFloatVal(config.getConfig("positionZ"));
    character = new Character(0.0f, initX, initY, initZ);


    /************************** 开始TCP ********************/
    int opt = 1;

    bzero(&server_addr,sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

    // create a socket
    server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0) {
        printf("Create Socket Failed!");
        exit(1);
    }

    // bind socket to a specified address
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))) {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
        exit(1);
    }

    // listen a socket
    if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE)) {
        printf("Server Listen Failed!");
        exit(1);
    }
    // accept socket from client
    socklen_t length = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
    if( client_socket < 0) {
        printf("Server Accept Failed!\n");
        //break;
    }
}

/* The function called when our window is resized (which should't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height) {
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
        Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

//GLfloat positionX = 27.0f;

void drawGrass() {
    GLfloat sceneroty;
    sceneroty = 360.0f - character->getYRot();
    // 首先绘制草坪
    glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(grassTexture));

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == '*' || temp == 'B' || temp == 'b') {
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-character->getPositionX(), 0.0f-character->getPositionY(), i*(-4.0f)-character->getPositionZ());
                glCallList(grass);
            }
        }
    }
}

void drawBuilding() {
    GLfloat sceneroty;
    sceneroty = 360.0f - character->getYRot();

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == 'B') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(building1Texture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-character->getPositionX(), 0.0f-character->getPositionY(), i*(-4.0f)-character->getPositionZ());
                glCallList(building1);
            } else if(temp == 'b') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(building2Texture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-character->getPositionX(), 0.0f-character->getPositionY(), i*(-4.0f)-character->getPositionZ());
                glCallList(building2);
            }
        }
    }
}
void drawRoad() {
    GLfloat sceneroty;
    sceneroty = 360.0f - character->getYRot();

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == '|') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(roadTexture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-character->getPositionX(), 0.0f-character->getPositionY(), i*(-4.0f)-character->getPositionZ());
                glCallList(road);
            } else if(temp == '-' || temp == '+') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(roadTexture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-character->getPositionX(), 0.0f-character->getPositionY(), i*(-4.0f)-character->getPositionZ());
                glCallList(horRoad);
            }
        }
    }
}
/*
char floatStr[64];
char str[] = "speed: ";
void printInfo() {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glRasterPos2i(0, 0);

    //逐个显示字符串中的每个字符
    const unsigned char* t = reinterpret_cast<const unsigned char *>("statue");
    //glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);

}
*/

/* The main drawing function. */
void DrawGLScene() {


    /// step 0: TCP 接受消息
    bzero(each_line_data, BUFFER_SIZE);
    memset(each_line_data, 0, sizeof(each_line_data));
    int length = recv(client_socket, each_line_data, BUFFER_SIZE, 0);
    if (length < 0) {
        //rprintf("Server Receive Data Failed!\n");
        //break;
    }else{
        //printf("%s\n\n", each_line_data);
        int wheel, power, stop;
        sscanf(each_line_data, "%d&%d&%d", &wheel, &power, &stop);
        printf("wheel:%d power:%d stop:%d\n", wheel, power/25, stop);
        character->turnWheel(wheel*5);
        character->stepOnTheAccelerator(power/25);
        //character->stepOnTheBrake(stop/25);
    }

    if('q' == each_line_data[0]) {
        printf("Quit from client!\n");
        //break;
    }

    // signal:1 mark:0 2015.000000
    //printf("%d %d\n", mark, mark_tmp);
    //sscanf(each_line_data, "signal:%d mark:%d %f", &signal, &mark, &eeg_data_buffer[eeg_data_pos]);

    /// step1: 首先刷新主视角状态
    character->refreshCharacterStatus();
    //printf("positionX: %f, positionY: %f, positionZ: %f, yRot: %f\n", character->getPositionX(), character->getPositionY(), character->getPositionZ(), character->getYRot());

    /// step2: 画图
    // 将背景色设置为天蓝色
    glClearColor(0.251f, 0.251f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

    glEnable(GL_TEXTURE_2D);

    drawGrass();
    drawBuilding();
    drawRoad();
    //printInfo();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) {
    /* avoid thrashing this procedure */
    //usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) {
        fclose(fp);
        /* shut down our window */
        glutDestroyWindow(window);

        /* exit the program...normal termination. */
        exit(0);
    }

    /*else{
        printf ("Key %d pressed. No action there yet.\n", key);
        exit(0);
    }*/
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y) {
    /* avoid thrashing this procedure */
    //usleep(100);

    switch (key) {
    case GLUT_KEY_UP:
        //runLenMax += 0.025f;
        //if(runLenMax >= 1.0f) runLenMax = 1.0f;
        character->stepOnTheAccelerator(200);
        character->turnWheel(0);
        break;

    case GLUT_KEY_DOWN:
        //character->stepOnTheAccelerator(-200);
        //character->turnWheel(0);
        /// 作弊代码
        character->setGoAheadRate(0.0f);
        character->setGoAheadAc(0.0f);
        character->turnWheel(0);
        character->setYRot(0.0f);
        break;

    case GLUT_KEY_LEFT:
        //yrot += 1.5f;
        character->turnWheel(800);
        break;

    case GLUT_KEY_RIGHT:
        //yrot -= 1.5f;
        character->turnWheel(-800);
        break;

    default:
        //printf ("Special key %d pressed. No action there yet.\n", key);

        //character->turnWheel((-400)*(key-6)+200);
        break;
    }
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("EEG Virtual Driving Environment");
    glutDisplayFunc(&DrawGLScene);
    //glutFullScreen();
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);
    glutSpecialFunc(&specialKeyPressed);
    InitGL(640, 480);
    glutMainLoop();

    return 0;
}
