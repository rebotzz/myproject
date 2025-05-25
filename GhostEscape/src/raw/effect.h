#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "../core/object_world.h"
#include "../affiliate/sprite_anim.h"
#include <functional>

class Scene;

// 特效，只播放一次，播放结束就失效
class Effect : public ObjectWorld
{
protected:
    SpriteAnim* anim_ = nullptr;        // 特效动画
    std::function<void()> on_finished_; // 特效结束时执行逻辑
        
public:
    Effect(Object* parent, const glm::vec2& position, ResID tex_anim, int total_frame_count, float scale = 1.0f, float frame_interval = 0.1f);
    ~Effect() = default;

    virtual void update(float dt);
    void setOnFinished(const std::function<void()>& callback) { on_finished_ = callback; }
};





#endif // _EFFECT_H_