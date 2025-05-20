#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "object_world.h"

// 角色
class Actor : public ObjectWorld
{
protected:
    glm::vec2 velocity_ = glm::vec2(0);
    float speed_ = 300.0f;

public:
    Actor() = default;
    virtual ~Actor() {};


    // getters and setters
    float getSpeed() const { return speed_; }
    void setSpeed(float val) { speed_ = val; }
};



#endif // _ACTOR_H_