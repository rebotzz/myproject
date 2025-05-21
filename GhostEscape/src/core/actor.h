#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "object_world.h"
#include "scene.h"

class Status;
class CollideBox;

// 角色
class Actor : public ObjectWorld
{
protected:
    glm::vec2 velocity_ = glm::vec2(0);
    float speed_ = 300.0f;
    Status* status_ = nullptr;  // 状态
    CollideBox* collide_box_ = nullptr; // 碰撞盒子

public:
    Actor() = default;
    virtual ~Actor() {};


    // getters and setters
    float getSpeed() const { return speed_; }
    void setSpeed(float val) { speed_ = val; }
    Status* getStatus() const { return status_; }
    CollideBox* getCollideBox() const { return collide_box_; }
    Scene* getScene() const { return dynamic_cast<Scene*>(parent_); }
};



#endif // _ACTOR_H_