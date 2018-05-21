#ifndef WORLDMAP_H
#define WORLDMAP_H


class WorldMap {
public:
    WorldMap();
    WorldMap(const char* fileName);      // 传入地图文件名, 读取矩阵
    virtual ~WorldMap();
    void initWorldMap(const char* fileName);
    void prinfWorldMap();
    char** getWorldMap() {
        return worldMap;
    }
    int getLength() {
        return length;
    }
    int getWidth() {
        return width;
    }
    char getWorldMap(int row, int col){
        return (this->worldMap)[row][col];
    }

protected:

private:
    char **worldMap;
    int length;
    int width;
    /************************************************
    * function : initLengthAndWidth
    * description : 根据 txt 文件中的矩阵, 获取矩阵的大小(m, n可以不等)
    * param : filename 文件在磁盘中的位置(str)
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-15 17:23:46
    *************************************************/
    void initLengthAndWidth(const char* fileName);
    /************************************************
    * function : initWordMapMatrix
    * description : 将文件中的矩阵读取进矩阵,
    * 调用该函数之前, 需要先调用initLengAndWidth对矩阵长宽进行初始化
    * param : filename 文件在磁盘中的位置(str)
    * output : 暂无
    * return : 暂无
    * author : yunyao
    * date : 2018-03-15 17:23:46
    *************************************************/
    void initWordMapMatrix(const char* fileName);
};

#endif // WORLDMAP_H
