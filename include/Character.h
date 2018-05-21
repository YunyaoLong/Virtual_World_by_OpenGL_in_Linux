#ifndef CHARACTER_H
#define CHARACTER_H

/*************************
 * Module Name : Character
 * Module Date : 2018-03-18 11:39:40
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 角色移动基本类
 * Version : V1.0
 * History :
 *  1. 初步完成角色移动类 2018-02-24 20:50:22
**************************/


#include <GL/gl.h>
class Character {
public:
    Character();
    Character(GLfloat goAheadAc, GLfloat positionX, GLfloat positionY);
    Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat xRot, GLfloat yRot);
    //virtual ~Character();

    GLfloat getGoAheadRate() {
        return goAheadRate;
    }
    void setGoAheadRate(GLfloat val) {
        goAheadRate = val;
    }
    GLfloat getGoAheadAc() {
        return goAheadAc;
    }
    void setGoAheadAc(GLfloat val) {
        goAheadAc = val;
    }
    GLfloat getPositionX() {
        return positionX;
    }
    void setPositionX(GLfloat positionX) {
        this->positionX = positionX;
    }
    GLfloat getPositionY() {
        return positionY;
    }
    void setPositionY(GLfloat positionY) {
        this->positionY = positionY;
    }
    GLfloat getXRot() {
        return this->xRot;
    }
    void setXRot(GLfloat xRot) {
        this->xRot = xRot;
    }
    GLfloat getYRot() {
        return this->yRot;
    }
    void setYRot(GLfloat yrot) {
        this->yRot = yRot;
    }
    void turnLeft(int);         // 传入方向盘的数据
    void turnRight(int);
    void stepOnTheAccelerator(int);     // 踩油门
    void stepOnTheBrake(int);           // 踩刹车
    // TODO (yunyao#1#): 完成基本的运动状态刷新功能. ...
    //之后尝试完成汽车四轮运动动力学模型.
    void refreshCharacterStatus();

protected:

private:
    GLfloat goAheadRate;        // 步进速度
    GLfloat goAheadAc;          // 步进加速度
    GLfloat positionX, positionY;   // 地图上所在的横坐标, 纵坐标
    GLfloat xRot, yRot;         // 沿着x轴和y轴的转向角度, xRot用来控制仰视俯视, yRot用来控制左右视角
    GLfloat angleSpeed;         // 向左角速度, 向右角速度
    GLfloat wheelAngle;         // 车轮角度
};

#endif // CHARACTER_H
