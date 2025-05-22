#ifndef _UI_CURSOR_H_
#define _UI_CURSOR_H_

#include "../core/object_screen.h"
#include "../core/sprite.h"

class UICursor : public ObjectScreen
{
protected:
    Sprite* cursor_1_ = nullptr;
    Sprite* cursor_2_ = nullptr;
    Sprite* current_cursor_ = nullptr;
    float timer_ = 0.0f;
    float time_change_sprite_cd_ = 1.0f;

public:
    UICursor(Object* parent, ResID tex_1, ResID tex_2, AchorMode mode = AchorMode::TOP_LEFT);

    virtual void update(float dt) override;

};

#endif // _UI_CURSOR_H_