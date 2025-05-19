#include "sprite.h"
#include "object_screen.h"


Sprite *Sprite::createAndAddSpriteChild(ObjectScreen *parent, ResID tex_id, float scale, const glm::vec2 &offset, AchorMode mode)
{
    auto sprite = new Sprite();
    sprite->tex_ = Game::getInstance().getAssetStore().getTexture(tex_id);
    SDL_GetTextureSize(sprite->tex_, &sprite->tex_size_.x, &sprite->tex_size_.y);
    sprite->setSize(sprite->tex_size_ * scale);
    sprite->setAchorModeAndSize(mode, sprite->size_);
    sprite->setRelativeOffset(offset);
    if(parent)
    {
        sprite->parent_ = parent;
        parent->addChild(sprite);
    }
    return sprite;
}

void Sprite::render()
{
    auto render_pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    SDL_FRect src_rect = {0.0f, 0.0f, tex_size_.x, tex_size_.y};
    SDL_FRect dst_rect = {render_pos.x, render_pos.y, size_.x, size_.y};
    game_.renderTexture(tex_, &src_rect, &dst_rect);
}

