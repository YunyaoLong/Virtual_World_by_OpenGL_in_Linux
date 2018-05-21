#include "TimeTool.h"

TimeTool::TimeTool() {
    (this->countdown) = 0;
    TimeTool::initBeginTime();
    TimeTool::refreshCurrentTime();
}

TimeTool::TimeTool(long countdown) {
    (this->countdown) = countdown;
    TimeTool::initBeginTime();
    TimeTool::refreshCurrentTime();
}

void TimeTool::initBeginTime() {
    time(&beginTime);
    (this->finishTime) = (this->beginTime)+(this->countdown);
}

void TimeTool::refreshCurrentTime() {
    time(&currentTime);
}

time_t TimeTool::getBeginTime() {
    return this->beginTime;
}

time_t TimeTool::getCurrentTime() {
    TimeTool::refreshCurrentTime();
    return this->currentTime;
}

time_t TimeTool::getFinishTime() {
    return this->finishTime;
}

long TimeTool::getCountdown() {
    return this->countdown;
}

void TimeTool::setCountdown(long countdown) {
    this->countdown = countdown;
    (this->finishTime) = (this->beginTime)+(this->countdown);

}

string TimeTool::timeToStr(time_t* time) {
    return string(ctime(time));
}

void TimeTool::timeToTm(time_t time,tm* p_tm) {
    p_tm = localtime(&time);
}

void TimeTool::tmToTime(tm* p_tm, time_t* time) {
    *time = mktime(p_tm);
}

bool TimeTool::isOverTime() {
    TimeTool::refreshCurrentTime();
    return (difftime(finishTime, currentTime) <= 0.0f);
}
