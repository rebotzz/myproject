#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "object_affiliate.h"
#include "../core/object_screen.h"

class ObjectScreen;
// extern enum class ResID;

// 精灵图 (组件)
class Sprite : public ObjectAffiliate
{
protected:
    SDL_Texture* tex_ = nullptr;
    glm::vec2 tex_size_ = glm::vec2(0);         // tex_size_:原图片裁剪区域尺寸; ObjectAffiliate::size_屏幕绘制矩形尺寸
    glm::vec2 render_position_ = glm::vec2(0);  // 位置 = 父节点位置 + 偏移
    glm::vec2 render_percentage_ = glm::vec2(1.0f); // 图片绘制裁剪百分比，从左到右，从下到上
    float alpha_ = 1.0f;        // 透明度
    bool is_flip_ = false;      // 是否水平反转
    float angle_ = 0.0f;        // 旋转角度
    float is_showing_ = true;   // 是否渲染

public:
    Sprite(ObjectScreen* parent, ResID tex_id, AchorMode mode = AchorMode::CENTER, 
        const glm::vec2& scale = glm::vec2(1));

    virtual void update(float) override;
    virtual void render() override;

    // setters and getters
    virtual void setSize(const glm::vec2& size) override;
    // 一般不用手动设置，自动更新，手动更新可以避免切换动画图片时闪现
    void setRenderPosition(const glm::vec2& position) { render_position_ = position; }  
    const glm::vec2& getRenderPosition() const { return render_position_; }
    void setRenderPercentage(const glm::vec2& percentage) { render_percentage_ = percentage; }
    void setTextureAlpha(float alpha) { alpha_ = alpha; }
    void setFlip(bool flag) { is_flip_ = flag; }
    void setAngle(float angle) { angle_ = angle; }
    void setShowing(bool flag) { is_showing_ = flag; }
    bool getShowing() const { return is_showing_; }
    const glm::vec2& getTextureSize() const { return tex_size_; }
};


#endif // _SPRITE_H_