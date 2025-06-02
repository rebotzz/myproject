#include "ui_cursor.h"

UICursor::UICursor(Object *parent, ResID tex_1, ResID tex_2, AchorMode mode):ObjectScreen(parent)
{
    cursor_1_ = new Sprite(this, tex_1, mode, glm::vec2(1.5));
    cursor_2_ = new Sprite(this, tex_2, mode, glm::vec2(1.5));
    current_cursor_ = cursor_1_;
    cursor_2_->setActive(false);
}


void UICursor::update(float dt)
{
    ObjectScreen::update(dt);

    // 更新鼠标位置
    game_.getMouseState(render_position_);

    // 更新现实光标图案
    timer_ += dt;
    auto old_cursor = current_cursor_;
    if(timer_ < time_change_sprite_cd_ / 2.0f)
    {
        current_cursor_ = cursor_1_;
    }
    else if(timer_ < time_change_sprite_cd_)
    {
        current_cursor_ = cursor_2_;
    }
    else
    {
        timer_ = 0.0f;
    }

    if(old_cursor != current_cursor_)
    {
        current_cursor_->setActive(true);
        old_cursor->setActive(false);
        current_cursor_->setRenderPosition(old_cursor->getRenderPosition());
    }
}
