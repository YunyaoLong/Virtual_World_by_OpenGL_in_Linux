/*****************************
 * Module Name : HBuilding(Horizontal Building)
 * Module Date : 2018-03-07 11:43:06
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 绘制水平长方体,绘点为长方体底面的左上角(俯视)
 * Version : V1.0
 * History :
 *  1. 初步完成基本对象绘画功能
*****************************/

#ifndef __HBUILDING_H_
#define __HBUILDING_H_
#include <GL/gl.h>

class HBuilding {        // vertex coordinates - 3d and texture
private:
    GLfloat x, y, z;     // 3d coords.
    GLfloat u, v;        // texture coords
    static GLfloat drawMatrix[24][5];
public:
    /****************************************
    * function : 构造函数
    * description : 传入水平长方体顶面的的右下角点(俯视), 因为楼栋仅需要使用同一个纹理修饰,所以使用简单粗暴的方法
    * Calls :
    * Called By : // 调用本函数的函数清单
    * param : x, y, z(水平长方体顶面的的右下角点坐标), u, v(纹理的右上角) 注意坐标系的方向
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-07 11:10:54
    ****************************************/
    HBuilding(); // 仅用于调用静态成员变量时, 调用这个构造函数
    HBuilding(GLfloat x, GLfloat y, GLfloat z);
    HBuilding(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

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
    static void resetMatrix(GLfloat x, GLfloat y, GLfloat z);
    /****************************************
    * function : drawHRectangle
    * description : 根据构造时传入的水平长方体的右下角和纹理右上角进行绘制
    * param : 暂无
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-07 11:10:54
    ****************************************/
    void drawHBuilding();


    static void drawHBuilding(GLfloat x, GLfloat y, GLfloat z) {
        glBegin(GL_QUADS);
        // 上顶面，真面朝上
        glTexCoord2f(0.0f, z);
        glVertex3f(0.0f, y, 0.0f);   // 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, y, z);	// 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(x, y, z);	    // 右下角
        glTexCoord2f(x, z);
        glVertex3f(x, y, 0.0f);	        // 右上角

        // 下底面，正面向下
        glTexCoord2f(0.0f, z);
        glVertex3f(0.0f, 0.0f, z);	// 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);	    // 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(x, 0.0f, 0.0f);	        // 右下角
        glTexCoord2f(x, z);
        glVertex3f(x, 0.0f, z);	        // 右上角

        // 前面
        glTexCoord2f(0.0f, z);
        glVertex3f(0.0f, y, z);	// 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, z);	// 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(x, 0.0f, z);	        // 右下角
        glTexCoord2f(x, z);
        glVertex3f(x, y, z);	    // 右上角

        // 后面
        glTexCoord2f(0.0f, z);
        glVertex3f(x, y, 0.0f);	        // 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x, 0.0f, 0.0f);	        // 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);	    // 右下角
        glTexCoord2f(x, z);
        glVertex3f(0.0f, y, 0.0f);	// 右上角

        // 左边
        glTexCoord2f(0.0f, z);
        glVertex3f(0.0f, y, 0.0f);	// 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);	    // 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(0.0f, 0.0f, z);    // 右下角
        glTexCoord2f(x, z);
        glVertex3f(0.0f, y, z);	// 右上角

        // 右面
        glTexCoord2f(0.0f, z);
        glVertex3f(x, y, z);        // 左上角
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x, 0.0f, z);	        // 左下角
        glTexCoord2f(x, 0.0f);
        glVertex3f(x, 0.0f, 0.0f);	        // 右下角
        glTexCoord2f(x, z);
        glVertex3f(x, y, 0.0f);	        // 右上角
        glEnd();
    }
};

#endif // __HBUILDING_H_
