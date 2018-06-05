/*****************************
 * Module Name : Rectangle
 * Module Date : 2018-03-07 11:07:40
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 绘制水平长方形,绘点为长方向的左上角(俯视)
 * Version : V1.0
 * History :
 *  1. 初步完成基本对象绘画功能
*****************************/

#ifndef __RECTANGLE_H_
#define __RECTANGLE_H_
#include <GL/gl.h>

class Rectangle {        // vertex coordinates - 3d and texture
private:
    GLfloat x, y, z;     // 3d coords.
    GLfloat u, v;        // texture coords
public:
    /****************************************
    * function : 构造函数
    * description : 传入水平长方形的右下角点
    * Calls :
    * Called By : // 调用本函数的函数清单
    * param : x, y, z(右下角坐标), u, v(纹理的右上角) 注意坐标系的方向
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-07 11:10:54
    ****************************************/
    Rectangle();
    Rectangle(GLfloat x, GLfloat y, GLfloat z);
    Rectangle(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

    void setX(GLfloat x) {
        this->x = x;
    }
    void setY(GLfloat y) {
        this->y = y;
    }
    void setZ(GLfloat z) {
        this->z = z;
    }
    void setU(GLfloat u) {
        this->u = u;
    }
    void setV(GLfloat v) {
        this->v = v;
    }
    /****************************************
    * function : drawHRectangle
    * description : 根据构造时传入的水平长方形的右下角和纹理右上角进行绘制
    * Calls :
    * Called By : // 调用本函数的函数清单
    * param : 暂无
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-07 11:10:54
    ****************************************/
    void drawHRectangle();

    static void drawHRectangle(GLfloat x, GLfloat y, GLfloat z, GLfloat u = 1.0f, GLfloat v = 1.0f) {
        // cube without the top;
        glBegin(GL_QUADS);			// Bottom Face
        // 逆时针绘制
        // 这个部分存放了马路的渲染元素
        glTexCoord2f(0.0f, v);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, y, z);
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, y, z);
        glTexCoord2f(u, v);
        glVertex3f(x, 0.0f, 0.0f);
        glEnd();
    }

    /// 绘制水平横向的矩形
    static void drawHHRectangle(GLfloat x, GLfloat y, GLfloat z, GLfloat u = 1.0f, GLfloat v = 1.0f) {
        // cube without the top;
        glBegin(GL_QUADS);			// Bottom Face
        // 逆时针绘制
        // 这个部分存放了马路的渲染元素
        glTexCoord2f(u, v);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, v);
        glVertex3f(0.0f, y, z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x, y, z);
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, 0.0f, 0.0f);
        glEnd();
    }
};

#endif // __RECTANGLE_H_
