#include "Rectangle.h"

Rectangle::Rectangle() { // 仅用于调用静态成员变量时, 调用这个构造函数
}

Rectangle::Rectangle(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = 1.0f;
    this->v = 1.0f;
}

Rectangle::Rectangle(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    this->v = v;
}

void Rectangle::drawHRectangle() {
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
