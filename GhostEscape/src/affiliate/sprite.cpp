#include "sprite.h"
#include "../core/object_screen.h"

Texture::Texture(ResID id)
{
    texture = Game::getInstance().getAssetStore().getTexture(id);
    SDL_GetTextureSize(texture, &src_rect.w, &src_rect.h);
}

Sprite::Sprite(ObjectScreen *parent, ResID tex_id, AchorMode mode, 
    const glm::vec2& scale)
    :ObjectAffiliate(parent, mode)
    ,texture_(tex_id)
{
    setSize(glm::vec2(texture_.src_rect.w, texture_.src_rect.h) * scale);
}

void Sprite::render()
{
    SDL_SetTextureAlphaModFloat(texture_.texture, alpha_);
    auto pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    game_.renderTexture(texture_, pos, getSize(), percentage_);
    SDL_SetTextureAlphaModFloat(texture_.texture, 1.0f);
}

void Sprite::setTexture(const Texture &texture)
{
    texture_ = texture;
    size_ = glm::vec2(texture.src_rect.w, texture.src_rect.h);
}

glm::vec2 Sprite::getLeftTopPosition() const
{
    auto pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    return pos;
}
