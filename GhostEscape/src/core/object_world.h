#ifndef _OBJECT_WORLD_H_
#define _OBJECT_WORLD_H_

#include "object_screen.h"


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

    void updateRenderPosition(const glm::vec2& camera_position) 
        { render_position_ = worldPositionToRenderPosition(camera_position); }

    // setters and getters
    void setPosition(const glm::vec2& position) { world_position_ = position; }
    const glm::vec2& getPosition() const { return world_position_; }

protected:
    // 工具函数
    glm::vec2 worldPositionToRenderPosition(glm::vec2 camera_position) { return world_position_ - camera_position; }

};



#endif // _OBJECT_WORLD_H_