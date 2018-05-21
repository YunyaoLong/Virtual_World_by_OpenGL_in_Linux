/*************************
 * Module Name : Objects
 * Module Date : 2018-02-28 19:49:29
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 绘制对象(显示列表)
 * Version : V1.0
 * History :
 *  1. 初步完成基本对象绘画功能
**************************/

#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include "Rectangle.h"
#include "HBuilding.h"

GLuint road;             // storage for the display list
GLuint grass;        // storage for the 2nd display list
GLuint building1;        // storage for the 3rd display list
GLuint building2;        // storage for the 4th display list
GLuint children;        // storage for the 5th display
GLuint commonBox;      // 正常的,一般的箱子
GLuint barBox;         // 障碍物箱子

GLfloat width, height, depth; // 楼房的长宽高参数
// build the display list.
GLvoid BuildList() {
    Rectangle rectangle; // 用于绘制水平矩形
    HBuilding hBuilding;   // 用户绘制水平楼栋

    // 首先绘制马路
    road = glGenLists(6);              //开辟一个5个元素大小的显示列表(display list), 返回首地址指向的元素
    glNewList(road, GL_COMPILE);
    width = 4.0f, depth = 4.0f;
    //HRectangle *roadHRectangle = new HRectangle(width, 0.0f, depth); // 水平矩形的高度必须为0
    rectangle.drawHRectangle(width, 0.0f, depth); // 推荐使用静态成员函数, 减少new delete的时空消耗
    glEndList();

    // 接着绘制草坪
    grass = road + 1;
    glNewList(grass, GL_COMPILE);
    width = 4.0f, depth = 4.0f;
    rectangle.drawHRectangle(width, 0.0f, depth);
    glEndList();

    // 绘制楼栋1
    building1 = road + 2;                    // since we generated 2 lists, this is where the second is...1 GLuint up from cube.
    width = 1.0f, height = 5.0f, depth = 3.0f;
    glNewList(building1, GL_COMPILE);        // generate 2nd list (top of box).
    hBuilding.drawHBuilding(width, height, depth);
    glEndList();

    // 绘制楼栋2
    building2 = road + 3;                    // since we generated 2 lists, this is where the second is...1 GLuint up from cube.
    width = 1.0f, height = 3.0f, depth = 3.0f;
    glNewList(building2, GL_COMPILE);        // generate 2nd list (top of box).
    hBuilding.drawHBuilding(width, height, depth);
    glEndList();

    // 绘制儿童
    children = road + 4;
    width = 1.0f;
    height = 1.0f;
    glNewList(children, GL_COMPILE);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, height, 0.0f);         // 左上角
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);	        // 左下角
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);	        // 右下角
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, height, 0.0f);	        // 右上角
    glEnd();
    glEndList();

    // 绘制路人木箱
    commonBox = road + 5;
    width = 0.5f, height = 0.5f, depth = 0.5f;
    glNewList(commonBox, GL_COMPILE);
    hBuilding.drawHBuilding(width, height, depth);
    glEndList();
}


#endif // __OBJECTS_H_
