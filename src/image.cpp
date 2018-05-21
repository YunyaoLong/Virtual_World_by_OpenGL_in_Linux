#include "image.h"

Image::Image() {
    this->sizeX = 1L;
    this->sizeY = 1L;
    this->size = sizeX*sizeY*3; // 3通道
    this->data = NULL;
}

Image::Image(unsigned long sizeX, unsigned long sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->size = sizeX*sizeY*3; // 3通道
    this->data = NULL;
}

Image::Image(unsigned long sizeX, unsigned long sizeY, char* data) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->size = sizeX*sizeY*3; // 3通道
    this->data = data;
}

Image::~Image() {
    // 析构时,注意释放data指向的内存空间
    delete[] this->data;
    this->data = NULL; // 防止野指针
}

void Image::calculateSize() {
    this->size = sizeX*sizeY*3; // 3通道
}


int Image::imageLoad(const char *filename) {
    FILE *file;
    unsigned long i;                    // standard counter.
    char temp;                          // temporary color storage for bgr-rgb conversion.
    unsigned short int planes;          // number of planes in image (must be 1) 图像的位面数
    unsigned short int bpp;             // number of bits per pixel (must be 24) 分辨率

    // 保证文件存在
    if ((file = fopen(filename, "rb"))==NULL) {
        //printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR); // 跳过bmp文件头部

    // 读宽
    if ((i = fread(&sizeX, 4, 1, file)) != 1) {
        //printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // 读高
    if ((i = fread(&sizeY, 4, 1, file)) != 1) {
        //printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    calculateSize();

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        //printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (planes != 1) {
        //printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        //printf("Error reading bpp from %s.\n", filename);
        return 0;
    }
    if (bpp != 24) {
        //printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    this->data = new char[this->size];
    if (this->data == NULL) {
        //printf("Error allocating memory for color-corrected image data");
        return 0;
    }

    if ((i = fread(this->data, this->size, 1, file)) != 1) {
        //printf("Error reading image data from %s.\n", filename);
        return 0;
    }

    for (i=0; i<(this->size); i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = this->data[i];
        this->data[i] = this->data[i+2];
        this->data[i+2] = temp;
    }

    // we're done.
    return 1;
}
