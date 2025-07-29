#include "sprite_anim.h"
#include "../core/object_screen.h"


SpriteAnim::SpriteAnim(ObjectScreen *parent, ResID tex_id, int frame_count, AchorMode mode, const glm::vec2 &scale, 
    float frame_interval, float is_loop)
    :Sprite(parent, tex_id, mode, scale)
    ,total_frame_count_(frame_count)
    ,frame_interval_(frame_interval)
    ,is_loop_(is_loop)
    ,tex_id_(tex_id)
{
    // 计算每一帧的尺寸,原图和绘制尺寸
    glm::vec2 frame_size = {tex_size_.x / static_cast<float>(frame_count), tex_size_.y};
    setSize(frame_size * scale);
    for(int i = 0; i < frame_count; ++i)
    {
        frames_.emplace_back(tex_, SDL_FRect{i * frame_size.x, 0, 
            frame_size.x, tex_size_.y});
    }
    // render_position_ = parent->getRenderPosition() + offset_;
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
    if(is_finished_ || !is_showing_ || frame_idx_ >= total_frame_count_) return;
    SDL_FRect dst_rect = { render_position_.x, render_position_.y, size_.x, size_.y};
    game_.renderTexture(frames_[frame_idx_].tex_, &frames_[frame_idx_].src_rect_, &dst_rect, angle_, is_flip_);
}