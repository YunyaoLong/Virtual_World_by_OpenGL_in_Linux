/*************************
 * Module Name : image
 * Module Date : 2018-02-24 20:42:41
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 加载图像基本类
 * Version : V1.0
 * History :
 *  1. 初步完成图像加载类 2018-02-24 20:50:22
**************************/

// 防止交叉引用
#ifndef __IMAGE_H_
#define __IMAGE_H_

// 图片基本类 - 长宽和文件指针
#include <stdio.h>
//#include <string.h>

class Image {
private:
    unsigned long sizeX;
    unsigned long sizeY;
    unsigned long size;                 // 图片字节(byte)大小,3通道
    char *data;

public:

    Image();
    Image(unsigned long sizeX, unsigned long sizeY);
    Image(unsigned long sizeX, unsigned long sizeY, char* data);
    ~Image();

    void calculateSize();

    void setSizeX(unsigned long sizeX) {
        this->sizeX = sizeX;
        calculateSize(); // 重新计算图片大小
    }

    void setSizeY(unsigned long sizeY) {
        this->sizeY = sizeY;
        calculateSize(); // 重新计算图像大小
    }

    void setData(char *data) {
        this->data = data;
    }

    unsigned long getSizeX() {
        return this->sizeX;
    }

    unsigned long getSizeY() {
        return this->sizeY;
    }

    char* getData() {
        return this->data;
    }

    /************************************************
    * function : imageLoad
    * description : 装载图像进入 data 指针指向的内存空间
    * Calls : calculateSize, fopen, fread, fseek
    * Called By : // 调用本函数的函数清单
    * param : filename 文件在磁盘中的位置(str)
    * output : 暂无
    * return : 0 写入失败, 1 写入成功
    * author : yunyao
    * date : 2018-02-24 17:43:43
    *************************************************/
    // quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
    // See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
    int imageLoad(const char *filename);
};

#endif // __IMAGE_H_
