#ifndef _AFFILIATE_BAR_H_
#define _AFFILIATE_BAR_H_

#include "../affiliate/object_affiliate.h"

// 附属条
class AffilateBar : public ObjectAffiliate
{
protected:
    float percentage_ = 1.0f;          // 百分比
    SDL_FColor color_high_ = {0.0f, 1.0f, 0.0f, 1.0f};
    SDL_FColor color_middle_ = {0.0f, 1.0f, 1.0f, 1.0f};
    SDL_FColor color_low_ = {1.0f, 0.0f, 0.0f, 1.0f};

public:
    AffilateBar(Object *parent, const glm::vec2 &size, const glm::vec2 &relative_offset, AchorMode mode = AchorMode::BOTTOM_CENTER);
    ~AffilateBar() = default;


    virtual void render();

    // setters and getters
    void setPercentage(float val);
    void setHighColor(SDL_FColor color) { color_high_ = color; }
    void setMiddleColor(SDL_FColor color) { color_middle_ = color; }
    void setLowColor(SDL_FColor color) { color_low_ = color; }
};


#endif // _AFFILIATE_BAR_H_