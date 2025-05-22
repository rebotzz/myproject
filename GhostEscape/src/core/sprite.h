#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "object_affiliate.h"

class ObjectScreen;


// 精灵图 (组件)
class Sprite : public ObjectAffiliate
{
protected:
    SDL_Texture* tex_ = nullptr;
    glm::vec2 tex_size_ = glm::vec2(0);
    glm::vec2 render_position_ = glm::vec2(0);  // 位置 = 父节点位置 + 偏移
    glm::vec2 render_percentage_ = glm::vec2(1.0f); // 图片绘制裁剪百分比，从左到右，从下到上
    float alpha_ = 1.0f;        // 透明度

public:
    static Sprite* createAndAddSpriteChild(ObjectScreen* parent, ResID tex_id, const glm::vec2 scale = glm::vec2(0.0f), 
        const glm::vec2& offset = glm::vec2(0.0f), AchorMode mode = AchorMode::CENTER);

    virtual void update(float) override;
    virtual void render() override;

    // setters and getters
    // 一般不用手动设置，自动跟新，手动更新，避免切换动画时闪现
    virtual void setSize(const glm::vec2& size) override;
    void setRenderPosition(const glm::vec2& position) { render_position_ = position; }  
    const glm::vec2& getRenderPosition() const { return render_position_; }
    void setRenderPercentage(const glm::vec2& percentage) { render_percentage_ = percentage; }
    void setTextureAlpha(float alpha) { alpha_ = alpha; }
};


#endif // _SPRITE_H_