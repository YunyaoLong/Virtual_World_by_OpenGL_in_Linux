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
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep.
#include <time.h>
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

/* floats for x rotation, y rotation, z rotation */
//float xrot, yrot, zrot;

GLuint xloop;            // loop for x axis.
GLuint yloop;            // loop for y axis.

GLfloat xrot;            // rotates cube on the x axis.
GLfloat yrot;            // rotates cube on the y axis.


GLfloat runLen;        // 用来控制向前行驶
GLfloat runLenMax = 0.4f;  // 用来控制最高速度
GLfloat runLenMin = 0.0f;  // 用来控制最低速度
GLfloat barRunLen;
GLfloat childPos, barPos;
GLfloat barLeftPos = -3.0f, barRightPos = 1.0f; // 左右障碍物的位置
GLfloat childRunLen, childRunRate = 0.065f;  // 用来控制小孩的跑动, 其中第二个参数为障碍物出现的速度
// 步进速度以及步进加速度，其中不经速度上界为0.1f
// 步进速度和脚踏的角度有关，这里先默认为0.01f
GLfloat goAheadRate, goAheadAc = 0.001f;

// bdwAppear 用来存放不倒翁的3种出状态(消失,开始移动,静止), appear_mode用来存放不倒翁的出现方式(从左边出现还是随机出现)
char bdwAppear, appearMode, commonTextureMode;

// 左侧有12*4个单位的长度，右侧同样
// 一个int有32位，每一位便可以表示一个格子，每个格子长4个单位
int buildingLeftFlag, buildingRightFlag;

// 左侧有16*3个单位的长度，右侧同样, 用来标志某个位置的箱子是否为障碍箱子
int barLeftFlag, barRightFlag;

// 左侧有16*3个单位的长度，右侧同样, 用来标志模式5中, 该箱子是否需要显示(障碍箱子必定显示)
int visibleLeftFlag, visibleRightFlag;

bool leftOrRightFlag;
// disappear 用来存放障碍物将要消失的时间
int disappearTime;

char disappearBoxMode;

int squareLen;

/* TCP 链接  */
#define BUFFER_SIZE 1024
int serverSockfd, conn;
char buffer[BUFFER_SIZE];
bool serverInitFlag;


// 怪物出现概率
float monsterPro = 0.1;

// 记录怪兽出现的时间
time_t appear, appearTmp;

// 记录当前时间和一些参数
FILE *fp = NULL;
time_t now;
struct tm *tmNow;

WorldMap worldMap;
Config config;

#include <pthread.h>
// 定义一个接受eeg数据的数组
#define HELLO_WORLD_SERVER_PORT    4000
#define LENGTH_OF_LISTEN_QUEUE 20
#define CLIENT_IP "192.168.0.1"
#define MAX_EEG_DATA_BUFFER_SIZE 80
#define MAX_EEG_DAT 0.02
int signal, signalTmp;
int mark, markTmp;
float eegDataBuffer[MAX_EEG_DATA_BUFFER_SIZE];
int markPos[MAX_EEG_DATA_BUFFER_SIZE], signalPos[MAX_EEG_DATA_BUFFER_SIZE];
int eegDataPos;
float data1, data2, data3;
pthread_t writeEegDataBufferThread;
struct sockaddr_in serverAddr;
int serverSocket;
struct sockaddr_in clientAddr;
int clientSocket;
socklen_t length;
char eachLineData[BUFFER_SIZE];


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

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);


    serverInitFlag = false;
    squareLen = 0;
    runLen = 0.0f;
    barRunLen = 0.0f;
    goAheadRate = 0.0f;
    childRunLen = -3.0f;
    childPos = -7.0f;
    barLeftPos = -3.0f;
    barRightPos = 1.0f;
    //buildingLeftFlag = 0b0;
    buildingLeftFlag = 0b011000000101;
    buildingRightFlag = 0b000010101001;
    barLeftFlag = 0b111111111111111111;
    barRightFlag = 0b111111111111111111;
    srand(time(NULL));
    // 初始化出现时间
    time(&appear);
    time(&appearTmp);
    // 刚开始是不会出现不倒翁的
    bdwAppear = 0;
    // 默认从左侧出现不倒翁
    appearMode = 0;
    // 清空eeg数据数组的内存
    memset(eegDataBuffer, 0, sizeof(eegDataBuffer));
    memset(markPos, 0, sizeof(markPos));
    memset(signalPos, 0, sizeof(signalPos));
    eegDataPos = 0;
    disappearTime = 7;
    // 盒子默认均显示
    disappearBoxMode = 1;
    // 正常盒子默认纹理为0
    commonTextureMode = 0;

    fp = fopen("rate_recorder.txt", "a+"); // a+意味着在文本最后追加
    fp = fopen("recorder.txt", "a+"); // a+意味着在文本最后追加

    config.initConfig("conf/sysProperties.cfg");
    //config.printConfig();             // 检查配置文件是否读入正确

    worldMap.initWorldMap("Data/WorldMap.txt");
    //worldMap.prinfWorldMap();
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height) {
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
        Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// 传入参数 当前位置, 步进长度, 上届, 下届
// 返回数据,越界返回false, 否则返回true
bool moveFunction(GLfloat &pos, GLfloat addLen, GLfloat maxLen, GLfloat minLen) {
    // 首先检查该操作是否会越界
    if(pos + addLen > maxLen || pos + addLen < minLen) return false;
    else pos += addLen;

    return true;
}

GLfloat positionX = 27.0f;

void drawGrass() {
    GLfloat sceneroty;
    sceneroty = 360.0f - yrot;
    // 首先绘制草坪
    glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(grassTexture));

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == '*' || temp == 'B' || temp == 'b') {
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-positionX, -1.0f, i*(-4.0f)+runLen);
                glCallList(grass);
            }
        }
    }
}

void drawBuilding() {
    GLfloat sceneroty;
    sceneroty = 360.0f - yrot;

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == 'B') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(building1Texture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-positionX, -1.0f, i*(-4.0f)+runLen);
                glCallList(building1);
            } else if(temp == 'b') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(building2Texture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-positionX, -1.0f, i*(-4.0f)+runLen);
                glCallList(building2);
            }
        }
    }
}
void drawRoad() {
    GLfloat sceneroty;
    sceneroty = 360.0f - yrot;

    glRotatef(sceneroty, 0, 1.0f, 0);
    for(int i = 0; i<worldMap.getLength(); ++i) {
        for(int j = 0; j<worldMap.getWidth(); ++j) {
            char temp = worldMap.getWorldMap(i, j);
            if(temp == '|') {
                // 首先绘制草坪
                glBindTexture(GL_TEXTURE_2D, texture->getGLTexture(roadTexture));
                glLoadIdentity();
                glRotatef(sceneroty, 0, 1.0f, 0);
                glTranslatef(j*4.0f-positionX, -1.0f, i*(-4.0f)+runLen);
                glCallList(road);
            }
        }
    }

}

/* The main drawing function. */
void DrawGLScene() {

    /// 画图

    // 将背景色设置为天蓝色
    glClearColor(0.251f, 0.251f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

    glEnable(GL_TEXTURE_2D);

    drawGrass();
    drawBuilding();
    drawRoad();

    /************场景移动开始***************/
    // 开始进行场景移动
    goAheadRate += goAheadAc;
    //  防止速度过快
    if(goAheadRate >= runLenMax) goAheadRate = runLenMax;
    if(goAheadRate <= runLenMin) goAheadRate = runLenMin;
    //runLen += goAheadRate;
    // 小孩的速度恒定
    //childRunLen += 0.1f;
    if(runLen >= 4.0f) {
        //runLen = 0.0f;
        buildingLeftFlag = (buildingLeftFlag<<1) & 0b111111111111;
        buildingRightFlag = (buildingRightFlag<<1) & 0b111111111111;
        // 左边有1/4的概率出现一栋楼房
        int randTemp = rand();
        if(randTemp%8 == 0) buildingLeftFlag |= 1;
        // 右边同样有1/4的概率出现一栋楼房(如果为4的倍数)
        if(randTemp%8 == 1) buildingRightFlag |= 1;

        childPos += 4.0f;
    }

    /*****************场景移动结束***********/

    glDisable(GL_TEXTURE_2D);
    // 0表示消失
    // since this is double buffered, swap the buffers to display what just got drawn.
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
    } else if(key == CHANGE_MODE) {
        // 有两种情况可能发生
        // 情况一: 只从左边出现
        // 情况二: 左右都有可能出现
        appearMode = (appearMode+1) % 2;
    } else if(key == DISAPPEAR_BOX) {
        disappearBoxMode = (disappearBoxMode+1) % 5;
    } else if(key == CHANGE_TEXTURE) {
        commonTextureMode = (commonTextureMode+1) % 2;
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
        runLenMax += 0.025f;
        if(runLenMax >= 1.0f) runLenMax = 1.0f;
        runLen += goAheadRate;
        break;

    case GLUT_KEY_DOWN:
        runLenMax -= 0.025f;
        if(runLenMax <= 0.0f) runLenMax = 0.0f;
        runLen -= goAheadRate;
        break;

    case GLUT_KEY_LEFT:
        childRunRate -= 0.01f;
        if(childRunRate <= 0.0f) childRunRate = 0.0f;
        yrot += 1.5f;
        break;

    case GLUT_KEY_RIGHT:
        childRunRate += 0.01f;
        if(childRunRate >= 1.0f) childRunRate = 1.0f;
        yrot -= 1.5f;
        break;

    default:
        //printf ("Special key %d pressed. No action there yet.\n", key);
        break;
    }
    time(&now);
    tmNow = gmtime(&now);
    fprintf(fp, "%d-%d-%d %d:%d:%d runLenMax: %f childRunRate: %f\n",
            tmNow->tm_year+1900, tmNow->tm_mon+1, tmNow->tm_mday, tmNow->tm_hour+8,
            tmNow->tm_min, tmNow->tm_sec, runLenMax, childRunRate);
}

pthread_t draw_thread;

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("EEG Virtual Driving Environment");
    glutDisplayFunc(&DrawGLScene);
    glutFullScreen();
    glutIdleFunc(&DrawGLScene);
    //glutIdleFunc(&IdleFun);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);
    glutSpecialFunc(&specialKeyPressed);
    InitGL(640, 480);
    glutMainLoop();

    return 0;
}
