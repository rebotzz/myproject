#ifndef _OBJECT_SCREEN_H_
#define _OBJECT_SCREEN_H_

#include "object.h"
#include "glm/glm.hpp"


// 屏幕对象
class ObjectScreen : public Object
{
protected:
    glm::vec2 render_position_;

public:
    ObjectScreen() { setObjectType(ObjectType::Screen); }
    ObjectScreen(Object* parent):Object(parent, ObjectType::Screen) {}
    virtual ~ObjectScreen() {}

    // getters and setters
    virtual void setRenderPosition(const glm::vec2& position) { render_position_ = position; }
    virtual const glm::vec2& getRenderPosition() { return render_position_; }
    virtual const glm::vec2& getPosition() const { return render_position_; }
};



#endif // _OBJECT_SCREEN_H_