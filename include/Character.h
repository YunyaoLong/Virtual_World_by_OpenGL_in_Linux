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
    Character(GLfloat goAheadAc, GLfloat positionX, GLfloat positionY, GLfloat positionZ);
    Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat xRot, GLfloat yRot);
    Character(GLfloat goAheadAc, GLfloat goAheadRate, GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat xRot, GLfloat yRot, GLfloat maxRate, GLfloat minRate);
    //virtual ~Character();
    GLfloat getMass();
    void setMass(GLfloat val);
    GLfloat getGoAheadRate();
    void setGoAheadRate(GLfloat val);
    GLfloat getGoAheadAc();
    void setGoAheadAc(GLfloat val);
    GLfloat getPositionX();
    void setPositionX(GLfloat positionX);
    GLfloat getPositionY();
    void setPositionY(GLfloat positionY);
    GLfloat getPositionZ();
    void setPositionZ(GLfloat positionZ);
    GLfloat getXRot();
    void setXRot(GLfloat xRot);
    GLfloat getYRot();
    void setYRot(GLfloat yrot);
    void setPosition();
    void setPosition(GLfloat, GLfloat, GLfloat);
    void turnWheel(int);         // 传入方向盘的数据
    void stepOnTheAccelerator(int);     // 踩油门
    void stepOnTheBrake(int);           // 踩刹车
    // TODO (yunyao#1#): 之后尝试完成汽车四轮运动动力学模型.
    void refreshCharacterStatus();

protected:

private:
    GLfloat mass;               // 车辆的质量
    GLfloat goAheadRate;        // 步进速度
    GLfloat goAheadAc;          // 步进加速度
    GLfloat minRate, maxRate;   // 最低速度, 最高速度
    GLfloat positionX, positionY, positionZ;   // 地图上所在的横坐标, 纵坐标, 深度坐标
    // 所有和角度相关的数据中单位均为°,范围-80°~80
    GLfloat xRot, yRot;         // 沿着x轴和y轴的转向角度, xRot用来控制仰视俯视, yRot用来控制左右视角
    GLfloat angleSpeed;         // 向左角速度, 向右角速度
    GLfloat wheelAngle;         // 车轮角度, 单位°,范围-80°~80
};

#endif // CHARACTER_H
