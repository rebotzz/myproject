#ifndef _TIMER_H_
#define _TIMER_H_

#include "object.h"

// 似乎没必要
class Timer : public Object
{
protected:
    float timer_ = 0.0f;
    float interval_ = 0.0f;
    bool is_loop_ = false;      // 似乎没必要

public:
    Timer(Object* parent, float interval, bool is_loop = false)
        :interval_(interval), is_loop_(is_loop)
    { 
        if(parent)
        {
            parent->addChild(this);
            setParent(parent);
        }
    }

    virtual void update(float dt) override
    {
        timer_ += dt;
        if(timer_ >= interval_) timer_ = interval_;
    }

    void restart() { timer_ = 0.0f; }
    // getters and setters
    float getProgress() const { return timer_ / interval_; }
    void setLoop(bool flag) { is_loop_ = flag; }
};

#endif // _TIMER_H_