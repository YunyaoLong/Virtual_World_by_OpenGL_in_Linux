/*************************
 * Module Name : image
 * Module Date : 2018-02-24 21:39:18
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 加载图像基本类
 * Version : V1.0
 * History :
 *  1. 初步完成纹理加载类 2018-02-24 20:50:22
**************************/

// 防止交叉引用
#ifndef __TEXTURE_H_
#define __TEXTURE_H_


#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <vector>

class Texture {
private :
    GLuint *textures;         // 7种纹理，分别包含两种楼栋，一个公路，以及草地, 3种障碍贴图
    int length; // 已装载纹理数
    int max_length; // 总可装载纹理数
public :

    Texture(int max_length) { // 构造函数必须要先制定总的纹理数, 以减少动态数组带来的资源消耗
        this->textures = new GLuint[max_length];
        this->length = 0;
        this->max_length = max_length;
    }

    ~Texture() {
        delete[] textures;
        textures = NULL; // 防止野指针
    }


    /**************************************************
    * function : loadGLTexture
    * description : 将图片转换成纹理并且存进内存空间
    * Calls :
    * Called By : // 调用本函数的函数清单
    * param : *image
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-02-24 17:43:43
    ***************************************************/
    void loadGLTexture(Image *image) {
        if((this->length) >= (this->max_length)) {
            printf("length out of range");
        }
        glGenTextures(1, &textures[this->length]);
        glBindTexture(GL_TEXTURE_2D, textures[this->length]);   // 2d texture (x and y size)

        // 更换滤波方式之后，出来的效果比上面的更好
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->getSizeX(), image->getSizeY(), GL_RGB, GL_UNSIGNED_BYTE, image->getData());

        ++(this->length);
    }

    GLuint getGLTexture(int index) {
        if(index >= (this->length)) {
            printf("index out of range");
        }
        return (this->textures)[index];
    }

    int getLength() {
        return this->length;
    }

    int getMaxLength() {
        return this->max_length;
    }
};

/* storage for one texture  */
Texture *texture;           // 7种纹理，分别包含两种楼栋，一个公路，以及草地, 3种障碍贴图

/**************************************************
* function : loadGLTextures
* description : 将图片转换成纹理并且顺序存进内存空间
*   在initGl中调用一次
*   增删纹理时, 需要同时修改Texture构造函数中的参数,保持和实际纹理数一致
*       同时需要增删枚举类
* Calls : loadGLTexture
* Called By : // 调用本函数的函数清单
* param : 暂无
* output : 暂无
* return : 暂无
* author : yunyao
* date : 2018-02-24 17:43:43
***************************************************/
void loadGLTextures() {
    // Load Texture
    Image *image;
    texture = new Texture(8);

    /* 载入道路*/
    image = new Image();
    if (!image->imageLoad("Data/road.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    /* 载入草地 */
    image = new Image();
    if (!image->imageLoad("Data/grass.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    /* 载入楼栋1 */
    image = new Image();
    if (image == NULL) {
        exit(0);
    }

    if (!image->imageLoad("Data/building.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    /* 载入楼栋2 */
    image = new Image();
    if (!image->imageLoad("Data/building2.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    /* 载入儿童 */
    image = new Image();
    if (!image->imageLoad("Data/bdw.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    // 载入木箱一,表示正常走路的障碍物
    image = new Image();
    if (!image->imageLoad("Data/crate.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    // 载入木箱一(亮),表示正常走路的障碍物
    image = new Image();
    if (!image->imageLoad("Data/muwenli3.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;

    // 载入木箱二,表示突然出现的障碍物
    image = new Image();
    if (!image->imageLoad("Data/muwenli2.bmp")) {
        exit(1);
    }
    texture->loadGLTexture(image);
    delete image;
}

enum TEXTURE_ENUM{ROAD_TEXTURE, GRASS_TEXTURE, BUILDING1_TEXTURE, BUILDING2_TEXTURE, BDW_TEXTURE, CRATE_TEXTURE, MUWENLI3_TEXTURE, MUWENLI2_TEXTURE}
    roadTexture = ROAD_TEXTURE,
    grassTexture = GRASS_TEXTURE,
    building1Texture = BUILDING1_TEXTURE,
    building2Texture = BUILDING2_TEXTURE,
    bdwTexture = BDW_TEXTURE,
    crateTexture = CRATE_TEXTURE,
    muwenli3Texture = MUWENLI3_TEXTURE,
    muwenli2Texture = MUWENLI2_TEXTURE
;

#endif // __TEXTURE_H_
