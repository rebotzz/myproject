#ifndef _TIMER_H_
#define _TIMER_H_

#include "../core/object.h"

// 似乎没必要
class Timer : public Object         // 创建的时候，默认为 !active
{
protected:
    float timer_ = 0;               // 当前计时值
    float interval_;                // 计时间隔
    bool time_out_ = false;         // 是否已超时
protected:
public:
    Timer(Object* parent, float interval);
    virtual void update(float dt) override;

    void start() { is_active_ = true; }     // 开始计时
    void stop() { is_active_ = false; }     // 停止计时
    bool timeOut();                         // 检查是否超时，并重置超时状态
    float getProgress() { return timer_ / interval_; }  // 获取进度（0-1）

    // getters and setters
    float getTimer() const { return timer_; }
    void setTimer(float timer) { timer_ = timer; }
    float getInterval() const { return interval_; }
    void setInterval(float interval) { interval_ = interval; }
};

Timer::Timer(Object* parent, float interval)
    :Object(parent)
{
    setInterval(interval);
    setActive(true);        // 创建时默认激活
}


void Timer::update(float dt)
{
    timer_ += dt;                   // 累加时间
    if (timer_ >= interval_) {      // 检查是否达到间隔
        timer_ = 0;                 // 重置计时器
        time_out_ = true;           // 标记为超时
    }
}

bool Timer::timeOut()
{
    if (time_out_) {
        time_out_ = false;          // 读取后重置超时状态
        return true;
    }
    return false;
}

#endif // _TIMER_H_