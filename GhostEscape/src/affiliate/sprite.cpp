#include "sprite.h"
#include "../core/object_screen.h"


Sprite *Sprite::createAndAddSpriteChild(ObjectScreen *parent, ResID tex_id, const glm::vec2 scale, const glm::vec2 &offset, AchorMode mode)
{
    auto sprite = new Sprite();
    sprite->tex_ = Game::getInstance().getAssetStore().getTexture(tex_id);
    SDL_GetTextureSize(sprite->tex_, &sprite->tex_size_.x, &sprite->tex_size_.y);
    sprite->setAchorModeAndSize(mode, glm::vec2{sprite->tex_size_.x * scale.x, sprite->tex_size_.y * scale.y});
    sprite->setRelativeOffset(offset);
    if(parent)
    {
        sprite->parent_ = parent;
        parent->safeAddChild(sprite);
        sprite->render_position_ = parent->getRenderPosition() + sprite->offset_;  
    }
    return sprite;
}

void  Sprite::update(float)
{
    // 更新渲染坐标
    render_position_ = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;    
}

void Sprite::render()
{
    assert(tex_ != nullptr && parent_ != nullptr);
    if(!is_showing_) return;
    SDL_FRect src_rect = {0.0f, 
        tex_size_.y * (1.0f - render_percentage_.y), 
        tex_size_.x * render_percentage_.x, 
        tex_size_.y * render_percentage_.y};
    SDL_FRect dst_rect = {render_position_.x, 
        render_position_.y + size_.y * (1.0f - render_percentage_.y), 
        size_.x * render_percentage_.x, 
        size_.y * render_percentage_.y};
    SDL_SetTextureAlphaModFloat(tex_, alpha_);
    game_.renderTexture(tex_, &src_rect, &dst_rect, angle_, is_flip_);
    SDL_SetTextureAlphaModFloat(tex_, 1.0f);
}

void Sprite::setSize(const glm::vec2& size)
{
    ObjectAffiliate::setSize(size);
    // 更新渲染坐标
    render_position_ = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;    
}