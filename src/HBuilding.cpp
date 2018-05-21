#include "HBuilding.h"

HBuilding::HBuilding() {
}

HBuilding::HBuilding(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

HBuilding::HBuilding(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    this->v = v;
}


void  HBuilding::drawHBuilding() {
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
