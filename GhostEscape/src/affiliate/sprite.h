#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "object_affiliate.h"
#include "../core/object_screen.h"

class ObjectScreen;

struct Texture
{
    SDL_Texture *texture = nullptr;
    SDL_FRect src_rect = {0, 0, 0, 0};
    float angle = 0;
    bool is_flip = false;
    Texture() = default;
    Texture(ResID id); 
};


// 精灵图 (组件)
class Sprite : public ObjectAffiliate
{
protected:
    Texture texture_;
    glm::vec2 percentage_ = glm::vec2(1.0f); // 图片绘制裁剪百分比，从左到右，从下到上
    float alpha_ = 1.0f;        // 透明度
    bool is_finish_ = false;     // 为了方便继承类SpriteAnim

public:
    Sprite(ObjectScreen* parent, ResID tex_id, AchorMode mode = AchorMode::CENTER, 
        const glm::vec2& scale = glm::vec2(1));

    virtual void render() override;

    // setters and getters
    void setTextureAlpha(float alpha) { alpha_ = alpha; }
    Texture getTexture() const { return texture_; }
    virtual void setTexture(const Texture &texture);
    void setFlip(bool is_flip) { texture_.is_flip = is_flip; }
    void setAngle(float angle) { texture_.angle = angle; }
    bool getFlip() const { return texture_.is_flip; }
    float getAngle() const { return texture_.angle; }
    bool getFinish() const { return is_finish_; }
    void setFinish(bool is_finish) { is_finish_ = is_finish; }
    glm::vec2 getPercentage() const { return percentage_; }
    void setPercentage(const glm::vec2& percentage) { percentage_ = percentage; }

    glm::vec2 getLeftTopPosition() const;
};


#endif // _SPRITE_H_