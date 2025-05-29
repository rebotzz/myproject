#include "sprite_anim.h"
#include "../core/object_screen.h"


SpriteAnim *SpriteAnim::createAndAddSpriteAnimChild(ObjectScreen* parent, ResID tex_id, int frame_count, float scale,
    float frame_interval, float is_loop, const glm::vec2 &relative_offset, AchorMode mode)
{
    auto sprite_anim = new SpriteAnim(frame_count, frame_interval, is_loop);
    sprite_anim->tex_ = Game::getInstance().getAssetStore().getTexture(tex_id);
    SDL_GetTextureSize(sprite_anim->tex_, &sprite_anim->tex_size_.x, &sprite_anim->tex_size_.y);
    sprite_anim->tex_size_.x /= frame_count;
    sprite_anim->setAchorModeAndSize(mode, sprite_anim->tex_size_);
    sprite_anim->setScale(glm::vec2(scale));
    sprite_anim->setRelativeOffset(relative_offset);
    if(parent)
    {
        sprite_anim->parent_ = parent;
        parent->safeAddChild(sprite_anim);
        sprite_anim->render_position_ = parent->getRenderPosition() + sprite_anim->offset_;
    }
    return sprite_anim;
}

void SpriteAnim::syncFrameTime(SpriteAnim *sprite_anim)
{
    frame_idx_ = sprite_anim->frame_idx_;
    timer_ = sprite_anim->timer_;
    is_flip_ = sprite_anim->is_flip_;
}

void SpriteAnim::update(float dt)
{
    if(is_finished_) return;
    Sprite::update(dt); 

    timer_ += dt;
    if(timer_ >= frame_interval_)
    {
        timer_ = 0;
        frame_idx_++;
        if(frame_idx_ >= total_frame_count_)
        {
            if(is_loop_) frame_idx_ = 0;
            else is_finished_ = true;
        }
    }
}

void SpriteAnim::render()
{
    assert(tex_ != nullptr && parent_ != nullptr);
    if(is_finished_ || !is_showing_) return;
    if(frame_idx_ >= total_frame_count_) return;
    SDL_FRect src_rect = { frame_idx_ * tex_size_.x, 0, tex_size_.x, tex_size_.y};
    SDL_FRect dst_rect = { render_position_.x, render_position_.y, size_.x, size_.y};
    game_.renderTexture(tex_, &src_rect, &dst_rect, angle_, is_flip_);
}