#include "Character.h"


Character::Character(){
    goAheadRate = 0.0f;        // 步进速度
    goAheadAc = 0.0f;          // 步进加速度
    positionX = 0.0f;    // 地图上所在的横坐标, 纵坐标
    positionY = 0.0f;
    xRot = 0.0f;         // 转向角度
    yRot = 0.0f;
    leftAngleSpeed = 0.0f;
    rightAngleSpeed = 0.0f;
}

Character::Character(GLfloat goAheadAc, GLfloat positionX, GLfloat positionY) {
    this->goAheadAc = goAheadAc;          // 步进加速度
    this->goAheadRate = 0.0f;
    this->positionX = positionX;    // 地图上所在的横坐标, 纵坐标
    this->positionY = positionY;
    xRot = 0.0f;         // 转向角度
    yRot = 0.0f;
    leftAngleSpeed = 0.0f;
    rightAngleSpeed = 0.0f;
}

Character::Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat xRot, GLfloat yRot) {
    this->goAheadAc = goAheadAc;          // 步进加速度
    this->goAheadRate = goAheadRate;
    this->positionX = positionX;    // 地图上所在的横坐标, 纵坐标
    this->positionY = positionY;
    this->xRot = xRot;         // 转向角度
    this->yRot = yRot;
    leftAngleSpeed = 0.0f;
    rightAngleSpeed = 0.0f;
}
