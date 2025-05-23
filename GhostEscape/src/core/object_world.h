#ifndef _OBJECT_WORLD_H_
#define _OBJECT_WORLD_H_

#include "object_screen.h"
#include "scene.h"

// 世界对象
class ObjectWorld : public ObjectScreen
{
protected:
    glm::vec2 world_position_;
    
public:
    ObjectWorld() { setObjectType(ObjectType::World); };
    ObjectWorld(Object* parent):ObjectScreen(parent)  { setObjectType(ObjectType::World); };
    virtual ~ObjectWorld() {};

    virtual void update(float dt) override;

    // setters and getters
    const glm::vec2& getPosition() const { return world_position_; }
    // 同时修改渲染、世界坐标
    virtual void setRenderPosition(const glm::vec2& position) override; 
    void setPosition(const glm::vec2& position);    

protected:
    // 工具函数
    void syncRenderPosition() { render_position_ = game_.getCurrentScene()->worldToScreen(world_position_); } 
};



#endif // _OBJECT_WORLD_H_