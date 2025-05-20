#ifndef _SPRITE_ANIM_H_
#define _SPRITE_ANIM_H_

#include "sprite.h"

// 精灵图动画
class SpriteAnim : public Sprite
{
protected:
    int total_frame_count_ = 1;
    int frame_idx_ = 0;
    float frame_interval_ = 0.1f;
    bool is_loop_ = true;
    bool is_flip_ = false;
    float timer_ = 0;   // 计时器

public:
    SpriteAnim(int total_frame_count, float frame_interval, bool is_loop)
        :total_frame_count_(total_frame_count)
        ,frame_interval_(frame_interval)
        ,is_loop_(is_loop)
    {}
    virtual ~SpriteAnim(){};

    virtual void update(float dt);
    virtual void render();

    static SpriteAnim* createAndAddSpriteAnimChild(ObjectScreen* parent, ResID tex_id, int frame_count, float scale = 1.0, 
        float frame_interval = 0.1f, float is_loop = true, const glm::vec2 &offset = glm::vec2(0), AchorMode mode = AchorMode::CENTER);

    // setters and getters
    float getFrameInterval() const { return frame_interval_; }
    void setFrameInterval(float interval) { frame_interval_ = interval; }
    void setLoop(bool flag) { is_loop_ = flag; }
    bool getIsFinished() const { return frame_idx_ >= total_frame_count_; }
    void setFlip(bool flag) { is_flip_ = flag; }
    void setFrameIdx(int idx) { frame_idx_ = idx; }
    int getFrameIdx() const { return frame_idx_; }
    void syncFrameTime(SpriteAnim* sprite_anim) { frame_idx_ = sprite_anim->frame_idx_; timer_ = sprite_anim->timer_; }
    void restart() { frame_idx_ = 0; timer_ = 0; }
};




#endif // _SPRITE_ANIM_H_