#include "Character.h"
#include <math.h>

const float piover180 = 0.0174532925f;

Character::Character() {
    this->goAheadRate = 0.0f;        // 步进速度
    this->goAheadAc = 0.0f;          // 步进加速度
    this->setPosition();
    this->xRot = 0.0f;         // 转向角度
    this->yRot = 0.0f;
    this->angleSpeed = 0.0f;
    this->wheelAngle = 0.0f;
    this->maxRate = 0.4f;
    this->minRate = 0.0f;
}

Character::Character(GLfloat goAheadAc, GLfloat positionX, GLfloat positionY, GLfloat positionZ) {
    this->goAheadAc = goAheadAc;          // 步进加速度
    this->goAheadRate = 0.0f;
    this->setPosition(positionX, positionY, positionZ);
    this->xRot = 0.0f;         // 转向角度
    this->yRot = 0.0f;
    this->angleSpeed = 0.0f;
    this->wheelAngle = 0.0f;
    this->maxRate = 0.4f;
    this->minRate = 0.0f;
}

Character::Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat xRot, GLfloat yRot) {
    this->goAheadAc = goAheadAc;          // 步进加速度
    this->goAheadRate = goAheadRate;
    this->setPosition(positionX, positionY, positionZ);
    this->xRot = xRot;         // 转向角度
    this->yRot = yRot;
    this->maxRate = 0.4f;
    this->minRate = 0.0f;
    this->angleSpeed = 0.0f;
    this->wheelAngle = 0.0f;
}

Character::Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat xRot, GLfloat yRot, GLfloat maxRate, GLfloat minRate) {
    this->goAheadAc = goAheadAc;          // 步进加速度
    this->goAheadRate = goAheadRate;
    this->setPosition(positionX, positionY, positionZ);
    this->xRot = xRot;         // 转向角度
    this->yRot = yRot;
    this->maxRate = maxRate;
    this->minRate = minRate;
    this->angleSpeed = 0.0f;
    this->wheelAngle = 0.0f;
}

GLfloat Character::getMass() {
    return mass;
}

void Character::setMass(GLfloat val) {
    mass = val;
}

GLfloat Character::getGoAheadRate() {
    return goAheadRate;
}

void Character::setGoAheadRate(GLfloat val) {
    goAheadRate = val;
}

GLfloat Character::getGoAheadAc() {
    return goAheadAc;
}

void Character::setGoAheadAc(GLfloat val) {
    goAheadAc = val;
}

GLfloat Character::getPositionX() {
    return positionX;
}

void Character::setPositionX(GLfloat positionX) {
    this->positionX = positionX;
}

GLfloat Character::getPositionY() {
    return positionY;
}

void Character::setPositionY(GLfloat positionY) {
    this->positionY = positionY;
}

GLfloat Character::getPositionZ() {
    return positionZ;
}

void Character::setPositionZ(GLfloat positionZ) {
    this->positionZ = positionZ;
}

GLfloat Character::getXRot() {
    return this->xRot;
}

void Character::setXRot(GLfloat xRot) {
    this->xRot = xRot;
}

GLfloat Character::getYRot() {
    return this->yRot;
}

void Character::setYRot(GLfloat yrot) {
    this->yRot = yRot;
}

void Character::setPosition() {
    this->positionX = 0.0f;    // 地图上所在的横坐标, 纵坐标
    this->positionY = 0.0f;
    this->positionZ = 0.0f;
}

void Character::setPosition(GLfloat positionX, GLfloat positionY, GLfloat positionZ) {
    this->setPositionX(positionX);
    this->setPositionY(positionY);
    this->setPositionZ(positionZ);
}

void Character::turnWheel(int wheelValue) {
    // TODO (yunyao#1#): 待确定方向盘数值和车轮角度之间的关系
    //wheelAngle = 1.5f*((int)wheelValue/200); // 这里可能涉及到

    // TODO (yunyao#1#): 通过公式求出当前车辆的转向角速度
    //angleSpeed = wheelAngle;
    double K = 500.0, L_reciprocal = 0.2;    // a1越大转向越慢,a2越大转向越快
    angleSpeed = ((this->goAheadRate) / (1+K*(this->goAheadRate)*(this->goAheadRate))) * (L_reciprocal*wheelValue);

}

void Character::stepOnTheAccelerator(int acValue) {
    // 先使用简单的加速控制
    // TODO (yunyao#1#): 使用公式求出发动机的牵引力.
    this->goAheadAc = 0.001f*((int)acValue/200);
}

void Character::stepOnTheBrake(int breakValue) {
    // 使用简单的刹车控制
    this->goAheadAc = (-0.001f)*((int)breakValue/200);
}
//#include <stdio.h>
void Character::refreshCharacterStatus() {

    /// step: 刷新速度
    goAheadRate += goAheadAc;
    // 前进速度控制
    if(goAheadRate >= maxRate) {
        goAheadRate = maxRate;
    }
    // 倒车速度控制
    if(goAheadRate <= (0.0f-maxRate)) {
        goAheadRate = (0.0f-maxRate);
    }

    /// step2: 刷新角度
    yRot += angleSpeed;

    /// step3: 刷新坐标
    this->positionX -= (float)sin(yRot*piover180) * goAheadRate;
    this->positionZ -= (float)cos(yRot*piover180) * goAheadRate;
    //printf("%f %f\n", this->positionX, this->positionZ);
}
