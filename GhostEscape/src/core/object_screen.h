#ifndef _OBJECT_SCREEN_H_
#define _OBJECT_SCREEN_H_

#include "object.h"
#include "glm/glm.hpp"


// 屏幕对象
class ObjectScreen : public Object
{
protected:
    glm::vec2 render_position_;
    Object* parent_ = nullptr;

public:
    ObjectScreen() = default;
    virtual ~ObjectScreen() {};

    // virtual void handleEvent(const SDL_Event& event) override;
    // virtual void update(float dt) override;
    // virtual void render() override;


    // getters and setters
    void setRenderPosition(const glm::vec2& position) { render_position_ = position; }
    const glm::vec2& getRenderPosition() const { return render_position_; }
    void setParent(Object* parent) { parent_ = parent; }
};



#endif // _OBJECT_SCREEN_H_