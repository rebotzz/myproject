#ifndef _OBJECT_WORLD_H_
#define _OBJECT_WORLD_H_

#include "object_screen.h"
#include "scene.h"

class CollideBox;

// 世界对象
class ObjectWorld : public ObjectScreen
{
protected:
    glm::vec2 world_position_;
    CollideBox* collide_box_ = nullptr; // 碰撞盒子

public:
    ObjectWorld() { setObjectType(ObjectType::World); };
    ObjectWorld(Object* parent):ObjectScreen(parent)  { setObjectType(ObjectType::World); };
    virtual ~ObjectWorld() {};

    virtual void update(float dt) override;

    // setters and getters
    virtual const glm::vec2& getRenderPosition() override;
    virtual const glm::vec2& getPosition() const override { return world_position_; }
    virtual void setRenderPosition(const glm::vec2& position) override;  // 同时修改渲染、世界坐标   
    void setPosition(const glm::vec2& position);    
    CollideBox* getCollideBox() const { return collide_box_; }

protected:
    void setCollideBox(CollideBox* box) { collide_box_ = box; }
    // 工具函数
    void syncRenderPosition() { render_position_ = game_.getCurrentScene()->worldToScreen(world_position_); } 
};



#endif // _OBJECT_WORLD_H_