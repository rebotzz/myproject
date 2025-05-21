#include "ui_bar.h"
#include "../core/object_screen.h"


void UIBar::render()
{
    auto render_pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    SDL_FColor color;
    if(percentage_ > 0.7f) color = {0.0f, 1.0f, 0.0f, 1.0f};
    else if(percentage_ > 0.4f) color = {0.0f, 1.0f, 1.0f, 1.0f};
    else color = {1.0f, 0.0f, 0.0f, 1.0f};
    game_.renderFillRect(render_pos, {size_.x * percentage_, size_.y}, color);
    game_.renderBoundary(render_pos, render_pos + size_, 3);
}

void UIBar::setPercentage(float val)
{
    percentage_ = val;
    if(percentage_ < 0.0f) percentage_ = 0.0f;
    else if(percentage_ > 1.0f) percentage_ = 1.0f;
}