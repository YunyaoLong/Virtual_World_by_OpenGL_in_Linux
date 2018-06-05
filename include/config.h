/****************************************************************************
 * Module Name : config
 * Module Date : 2018-03-13 16:38:41
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 加载配置文件基本类,
 *               配置文件的格式，以#作为行注释，配置的形式是key = value，中间可有空格，也可没有空格
 * Version : V1.0
 * History :
 *  1. 初步完成配置文件加载类 2018-02-24 20:50:22
 *  2. 添加str2bool, str2float, str2int 2018-03-16 10:01:59
 *  3. 完善str2int功能, 支持2进制(如0b11), 8进制(如011), 10进制(如11), 16进制(如0x11)字符串转int
 *     参考连接 : http://c.biancheng.net/cpp/html/3421.html
*****************************************************************************/
#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <string>
#include <map>
using namespace std;

#define COMMENT_CHAR '#'
#define TRUE_STR "true"
#define FALSE_STR "false"

class Config {
private:
    map<string, string> configMap;
public:
    Config();
    Config(const string &fileName);
    bool initConfig(const string &fileName);
    void printConfig();
    string getConfig(const string &keyStr);
};

bool getBoolVal(string boolStr);
float getFloatVal(string floatStr);
/****************************************
* function : getIntVal
* description : 将传入的intStr转换成intVal,
*  支持2进制(0b或者0B开头), 8进制(0开头)
*  10进制, 16进制(0x或者0X)
* param : intStr
* output : 暂无
* return : intVal
* author : yunyao
* date : 2018-03-07 11:10:54
****************************************/
int getIntVal(string intStr);
void float2str(float val, char* buffer);

#endif


/****************************************


int main()
{
    Config config("sysProperties.cfg");
    config.printConfig();

    Config config2;
    config2.readConfig("sysProperties.cfg");
    config2.printConfig();

    return 0;
}


******************************************/
