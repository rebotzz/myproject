#include "sprite_anim.h"
#include "../core/object_screen.h"
#include "../core/object_world.h"


SpriteAnim::SpriteAnim(ObjectScreen *parent, ResID tex_id, int frame_count, AchorMode mode, const glm::vec2 &scale, 
    float frame_interval, float is_loop)
    :Sprite(parent, tex_id, mode, scale)
    ,total_frame_count_(frame_count)
    ,frame_interval_(frame_interval)
    ,is_loop_(is_loop)
{
    // 计算每一帧的尺寸,原图和绘制尺寸
    texture_.src_rect.w /= static_cast<float>(frame_count);
    setSize(glm::vec2(texture_.src_rect.w, texture_.src_rect.h) * scale);
}

void SpriteAnim::syncFrameTime(SpriteAnim *sprite_anim)
{
    frame_idx_ = sprite_anim->frame_idx_;
    timer_ = sprite_anim->timer_;
    texture_.is_flip = sprite_anim->texture_.is_flip;
}



void SpriteAnim::update(float dt)
{
    if(is_finish_) return;

    timer_ += dt;
    if(timer_ >= frame_interval_)
    {
        timer_ = 0;
        frame_idx_++;
        if(frame_idx_ >= total_frame_count_)
        {
            if(is_loop_) frame_idx_ = 0;
            else is_finish_ = true;
        }
    }

    texture_.src_rect.x = texture_.src_rect.w * frame_idx_;
}

void SpriteAnim::reset()
{
    timer_ = 0;
    frame_idx_ = 0;
}
