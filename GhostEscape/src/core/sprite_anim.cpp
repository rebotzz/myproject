#include "sprite_anim.h"
#include "object_screen.h"


SpriteAnim *SpriteAnim::createAndAddSpriteAnimChild(ObjectScreen* parent, ResID tex_id, int frame_count, float scale,
    float frame_interval, float is_loop, const glm::vec2 &offset, AchorMode mode)
{
    auto sprite_anim = new SpriteAnim(frame_count, frame_interval, is_loop);
    sprite_anim->tex_ = Game::getInstance().getAssetStore().getTexture(tex_id);
    SDL_GetTextureSize(sprite_anim->tex_, &sprite_anim->tex_size_.x, &sprite_anim->tex_size_.y);
    sprite_anim->tex_size_.x /= frame_count;
    sprite_anim->setSize(sprite_anim->tex_size_ * scale);
    sprite_anim->setAchorModeAndSize(mode, sprite_anim->size_);
    sprite_anim->setRelativeOffset(offset);
    if(parent)
    {
        sprite_anim->parent_ = parent;
        parent->safeAddChild(sprite_anim);
    }
    return sprite_anim;
}

void SpriteAnim::update(float dt)
{
    Sprite::update(dt); 

    timer_ += dt;
    if(timer_ >= frame_interval_)
    {
        timer_ = 0;
        frame_idx_++;
        if(frame_idx_ >= total_frame_count_ && is_loop_)
        {
            frame_idx_ = 0;
        }
    }
}

void SpriteAnim::render()
{
    if(frame_idx_ >= total_frame_count_) return;
    SDL_FRect src_rect = { frame_idx_ * tex_size_.x, 0, tex_size_.x, tex_size_.y};
    SDL_FRect dst_rect = { render_position_.x, render_position_.y, size_.x, size_.y};
    game_.renderTexture(tex_, &src_rect, &dst_rect, 0.0, is_flip_);
}