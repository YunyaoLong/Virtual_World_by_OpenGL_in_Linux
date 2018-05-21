#include "WorldMap.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void readstr(FILE *f, char *str) {
    do {
        fgets(str, 255, f); // read the line
    } while ((str[0] == '/') || (str[0] == '\n'));
    return;
}

WorldMap::WorldMap(){
    this->worldMap = NULL;
    this->length = 0;
    this->width = 0;
}

WorldMap::WorldMap(const char* fileName) {
    this->initWorldMap(fileName);
}

WorldMap::~WorldMap() {
    for(int i = 0; i<length; ++i) {
        delete[] worldMap[i];
    }
    delete[] worldMap;
}

void WorldMap::initWorldMap(const char* fileName){
    initLengthAndWidth(fileName);
    initWordMapMatrix(fileName);
}

void WorldMap::initWordMapMatrix(const char* fileName) {
    FILE *filein;        // file to load the world from

    filein = fopen(fileName, "rt");

    // 纵坐标倒置读取矩阵, 方便后续操作
    for(int i=(this->length)-1; i>=0; --i) {
        readstr(filein, worldMap[i]);
    }
    fclose(filein);
}

void WorldMap::initLengthAndWidth(const char* fileName) {
    int MAXSIZE = 256, charCount = 0;
    FILE *fp;
    char s[MAXSIZE];

    fp=fopen(fileName, "rt");

    this->length=0, this->width = 0;
    while(fgets(s,MAXSIZE,fp)!=NULL) { //读行数
        this->length++;
        charCount += strlen(s);
    }
    this->width = strlen(s);
    fclose(fp);
    if((this->width)*(this->length) != charCount) {
        printf("illegal matrix size!");
        exit(1);
    } else {
        this->worldMap = new char*[(this->length)];
        for(int i = 0; i<(this->length); ++i) {
            (this->worldMap)[i] = new char[(this->width)];
        }
    }
}

void WorldMap::prinfWorldMap(){
    for(int i = 0; i<(this->length); ++i){
        printf("%s\n", (this->worldMap)[i]);
    }
}
