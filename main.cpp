//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit Jeff at www.demonews.com/hosted/nehe
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep.
#include "tcp_server.h"

/* ascii code for the escape key */
#define ESCAPE 27

/* The number of our GLUT window */
int window;

/* floats for x rotation, y rotation, z rotation */
//float xrot, yrot, zrot;

/* storage for one texture  */
GLuint texture[4];         // 4种纹理，分别包含两种楼栋，一个公路，以及草地

GLuint road;             // storage for the display list
GLuint grass;        // storage for the 2nd display list
GLuint building1;        // storage for the 2nd display list
GLuint building2;        // storage for the 2nd display list
GLuint xloop;            // loop for x axis.
GLuint yloop;            // loop for y axis.

GLfloat xrot;            // rotates cube on the x axis.
GLfloat yrot;            // rotates cube on the y axis.

int square_len;

/* TCP 链接  */
#define BUFFER_SIZE 1024
int server_sockfd, conn;
char buffer[BUFFER_SIZE];
bool server_init_flag;

// colors for boxes.
static GLfloat boxcol[5][3]= {
    {1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
};

// colors for tops of boxes.
static GLfloat topcol[5][3]= {
    {.5f,0.0f,0.0f},{0.5f,0.25f,0.0f},{0.5f,0.5f,0.0f},{0.0f,0.5f,0.0f},{0.0f,0.5f,0.5f}
};

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

// build the display list.
GLvoid BuildList() {

    // 首先绘制马路
    road = glGenLists(3);              // generate storage for 2 lists, and return a pointer to the first.
    glNewList(road, GL_COMPILE);       // store this list at location cube, and compile it once.

    // cube without the top;
    glBegin(GL_QUADS);			// Bottom Face
    // 逆时针绘制
    // 这个部分存放了马路的渲染元素
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 4.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
    glEndList();

    // 接着绘制草坪
    grass = road + 1;
    glNewList(grass, GL_COMPILE);        // generate 2nd list (top of box).
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 4.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(4.0f, 0.0f, 0.0f);
    glEnd();
    glEndList();


    // 绘制楼栋
    building1 = grass + 1;                    // since we generated 2 lists, this is where the second is...1 GLuint up from cube.
    glNewList(building1, GL_COMPILE);        // generate 2nd list (top of box).
    glBegin(GL_QUADS);
    GLfloat width = 1.0, height = 1.0, depth = 1.0;
    // 上顶面
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad

    // 下底面
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad

    // 前面
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad

    // 后面
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad

    // 左边
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad

    // 右面
    glTexCoord2f(width, depth); glVertex3f(-1.0f, 5.0f, 0.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(width, 0.0f);  glVertex3f(-1.0f, 5.0f, 1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);   glVertex3f( 0.0f, 5.0f, 1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, depth);  glVertex3f( 0.0f, 5.0f, 0.0f);	// Bottom Right Of The Texture and Quad
    glEnd();

    glEndList();
}

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL) {
        printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }

    for (i=0; i<size; i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }

    // we're done.
    return 1;
}

// Load Bitmaps And Convert To Textures
void LoadGLTextures() {
    // Load Texture
    Image *image1;

    /* 载入道路*/
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("Data/road.bmp", image1)) {
        exit(1);
    }

    // Create Texture
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

    // 更换滤波方式之后，出来的效果比上面的更好
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* 载入草地 */
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("Data/grass.bmp", image1)) {
        exit(1);
    }

    // Create Texture
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)

    // 更换滤波方式之后，出来的效果比上面的更好
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* 载入楼栋1 */
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("Data/building.bmp", image1)) {
        exit(1);
    }

    // Create Texture
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)

    // 更换滤波方式之后，出来的效果比上面的更好
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* 载入楼栋2 */
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("Data/building2.bmp", image1)) {
        exit(1);
    }

    // Create Texture
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);   // 2d texture (x and y size)

    // 更换滤波方式之后，出来的效果比上面的更好
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    free(image1);
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height) {        // We call this right after our OpenGL window is created.
    LoadGLTextures();				// Load The Texture(s)
    BuildList();                                // set up our display lists.
    glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping

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

    server_init_flag = false;
    square_len = 0;
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

/* The main drawing function. */
void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

    // 绘制公路
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    for(int i = 0; i<12; ++i){
        glLoadIdentity();
        glTranslatef(-3.0f,-1.0f, (i+1)*(-4.0f)); // 移动绘点
        glCallList(road);
    }
    // 绘制草地
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    for(int i = 0; i<12; ++i){
        for(int j = 0; j<12; ++j) {
            if(j == 7) continue;
            glLoadIdentity();
            glTranslatef(-4.0f*(j-6)+1,-1.0f, (i+1)*(-4.0f)); // 移动绘点
            glCallList(road);
        }
    }

    // 绘制草地
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glLoadIdentity();
    glTranslatef(-3.0,-1.0f, -4.0f); // 移动绘点
    glCallList(building1);

    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) {
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) {
        /* shut down our window */
        glutDestroyWindow(window);

        /* exit the program...normal termination. */
        exit(0);
    }
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y) {
    /* avoid thrashing this procedure */
    usleep(100);

    switch (key) {
    case GLUT_KEY_UP:
        xrot -= 0.2f;
        break;

    case GLUT_KEY_DOWN:
        xrot += 0.2f;
        break;

    case GLUT_KEY_LEFT:
        yrot += 0.2f;
        break;

    case GLUT_KEY_RIGHT:
        yrot -= 0.2f;
        break;

    default:
        printf ("Special key %d pressed. No action there yet.\n", key);
        break;
    }
}

void IdleFun() { // 回调函数，在控制台中的一些操作，需要在本部分进行控制
    //printf("test\n");
    if(!server_init_flag) {
        //square_len++;
        //if(square_len >= 100) square_len = 0;
        //printf("init\n");
        if(tcp_server_init(server_sockfd, conn)) printf("success\n");
        else printf("false\n");
        server_init_flag = true;
        glutPostRedisplay();
    } else {
        memset(buffer,0,sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer),0);
        for(int i = 0; i<len; ++i) {
            if(buffer[0] == 0x66) {
                square_len++;
                if(square_len >= 100) square_len = 100;
            } else if(buffer[0] == 0x64) {
                square_len--;
                if(square_len <= 0) square_len = 0;
            }
        }
        fputs(buffer, stdout);
        send(conn, buffer, len, 0);
        glutPostRedisplay();
    }
}

int main(int argc, char **argv) {
    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
       X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
    glutInit(&argc, argv);

    /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    /* get a 640 x 480 window */
    glutInitWindowSize(640, 480);

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);

    /* Open a window */
    window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);

    /* Go fullscreen.  This is as soon as possible. */
    glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);

    /* Register the function called when special keys (arrows, page down, etc) are pressed. */
    glutSpecialFunc(&specialKeyPressed);

    /* Initialize our window. */
    InitGL(640, 480);

    /* Start Event Processing Engine */
    glutMainLoop();

    return 1;
}
