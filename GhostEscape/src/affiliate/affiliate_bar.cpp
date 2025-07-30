#include "../affiliate/affiliate_bar.h"
#include "../core/object_screen.h"
#include "affiliate_bar.h"

AffilateBar::AffilateBar(Object *parent, const glm::vec2 &size, const glm::vec2 &relative_offset, AchorMode mode)
    :ObjectAffiliate(parent)
{
    setAchorModeAndSize(mode, size);
    setRelativeOffset(relative_offset);
}

void AffilateBar::render()
{
    auto render_pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    SDL_FColor color;
    if(percentage_ > 0.7f) color = color_high_;
    else if(percentage_ > 0.4f) color = color_middle_;
    else color = color_low_;
    game_.renderFillRect(render_pos, {getScaledSize().x * percentage_, getScaledSize().y}, color);
    game_.renderBoundary(render_pos, render_pos + getScaledSize(), 3);
}

void AffilateBar::setPercentage(float val)
{
    percentage_ = glm::clamp(val, 0.f, 1.f);
}