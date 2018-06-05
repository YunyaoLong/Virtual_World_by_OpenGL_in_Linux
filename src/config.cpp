#include "config.h"

//#include <fstream>
//#include <iostream>
//using namespace std;
#include <stdlib.h>
#include <stdio.h>

Config::Config() {
    configMap.clear();
}

Config::Config(const string &fileName) {
    initConfig(fileName);
}

bool isSpace(char c) {
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

bool isCommentChar(char c) {
    switch(c) {
    case COMMENT_CHAR:
        return true;
    default:
        return false;
    }
}

void trim(string & str) {
    if (str.empty()) {
        return;
    }
    int start_pos, end_pos;
    unsigned int i;
    for (i = 0; i < str.size(); ++i) {
        if (!isSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // 全部是空白字符串
        str = "";
        return;
    }

    start_pos = i;

    for (i = str.size() - 1; i >= 0; --i) {
        if (!isSpace(str[i])) {
            break;
        }
    }
    end_pos = i;

    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool analyseLine(const string & line, string & key, string & value) {
    if (line.empty())
        return false;
    int start_pos = 0, end_pos = line.size() - 1, pos;
    if ((pos = line.find(COMMENT_CHAR)) != -1) {
        if (0 == pos) {  // 行的第一个字符就是注释字符
            return false;
        }
        end_pos = pos - 1;
    }
    string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分

    if ((pos = new_line.find('=')) == -1)
        return false;  // 没有=号

    key = new_line.substr(0, pos);
    value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

    trim(key);
    if (key.empty()) {
        return false;
    }
    trim(value);
    return true;
}

bool Config::initConfig(const string & fileName) {
    configMap.clear();
    //ifstream infile(fileName.c_str());
    FILE *infile;
    infile=fopen(fileName.c_str(), "rt");
    if (!infile) {
        printf("file open error\n");
        return false;
    }
    string key, value;

    int MAXSIZE = 256;
    char line[MAXSIZE];

    while(fgets(line,MAXSIZE, infile)!=NULL) { //读行数
        //while (getline(infile, line)) {
        if (analyseLine(line, key, value)) {
            configMap[key] = value;
        }
    }

    fclose(infile);
    return true;
}

void Config::printConfig() {
    map<string, string>::const_iterator mIte = configMap.begin();
    for (; mIte != configMap.end(); ++mIte) {
        printf("%s=%s\n", (mIte->first).c_str(), (mIte->second).c_str());
    }
}

string Config::getConfig(const string &keyStr) {
    return configMap[keyStr];
}


bool getBoolVal(string boolStr) {
    return boolStr == TRUE_STR;
}

float getFloatVal(string floatStr) {
    return atof(floatStr.c_str());
}

int getIntVal(string intStr) {
    int base = 10;              // 进制标志
    string tmp;
    if(intStr.size() > 1) {     // 017 表示 8进制
        tmp = intStr.substr(0, 1);
        if(tmp == "0") {
            base = 8;
        }
    }
    if(intStr.size() > 2) {
        tmp = intStr.substr(0, 2);
        if(tmp == "0x" || tmp == "0X") {
            base = 16;
        } else if(tmp == "0b" || tmp == "0B") {
            base = 2;
        }
    }
    if(base == 2 || base == 16) {
        intStr = intStr.substr(2, intStr.size()-2);
    } else {
        intStr = intStr.substr(1, intStr.size()-1);
    }
    return strtol(intStr.c_str(), NULL, base);
}

void float2str(float val, char* buffer){
    snprintf(buffer, sizeof buffer, "%f", val);
}
