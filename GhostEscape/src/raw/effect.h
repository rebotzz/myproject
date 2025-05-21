#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "../core/sprite_anim.h"
#include <functional>

// 特效，只播放一次，播放结束就失效
class Effect : public SpriteAnim
{
protected:
    std::function<void()> on_finished_; // 特效结束时执行逻辑
        
public:
    Effect(int total_frame_count, float frame_interval): SpriteAnim(total_frame_count, frame_interval, false){}
    ~Effect() = default;
    static Effect* createAndAddEffectChild(Object* parent, ResID tex_id, int frame_count, float scale = 1.0, 
        const std::function<void()>& callback = []{}, float frame_interval = 0.1f);

    virtual void update(float dt);

    void setOnFinished(const std::function<void()>& callback);
};





#endif // _EFFECT_H_