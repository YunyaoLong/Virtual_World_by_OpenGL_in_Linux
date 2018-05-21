#ifndef __TIMETOOL_H_
#define __TIMETOOL_H_
/**
* reference : http://blog.csdn.net/water_cow/article/details/7521567
*/
/*****************************
 * Module Name : TimeTool(不十分精确的时间工具, 只精确到秒)
 * Module Date : 2018-03-14 20:33:38
 * Module Auth : yunyao
 * Email : yaoyl753951@gmail.com
 * Description : 时间工具, 用来获取系统时间以及计时等功能, 一般使用计时工具(注意在开始计时的地方需要initBeginTimr)
 * Version : V1.0
 * History :
 *  1. 完成时间工具轮子的基本功能
*****************************/

#include <time.h>
#include <string>
using namespace std;

class TimeTool {
private:
    time_t beginTime;   // 开始计时时间
    time_t finishTime;  // 终止时间
    time_t currentTime; // 当前时间
    long countdown;     // 计时时间
public:
    TimeTool();                 // 初始化为当前时间
    TimeTool(long countdown);   // 设置一个计时时间, 单位s
    void initBeginTime();       // 初始化起始时间为当前时间, 同时会刷新终止时间
    void refreshCurrentTime();  // 刷新当前时间
    time_t getBeginTime();      // 获取计时开始时间
    time_t getCurrentTime();    // 获取系统当前时间时间
    time_t getFinishTime();     // 获取计时结束时间
    long getCountdown();        // 获取倒计时总时长
    void setCountdown(long);    // 重新设置计时时间, 不会重置起始时间, 只会重置终止时间
    string timeToStr(time_t*);  // 获取time_t to str
    void timeToTm(time_t, tm*); // time_t 转 tm
    void tmToTime(tm*, time_t*);// tm 转 time_t
    bool isOverTime();          // 判断当前是否超时
};
#endif // __TIMETOOL_H_
