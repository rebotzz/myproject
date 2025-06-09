#include "effect.h"
#include "../core/scene.h"

Effect::Effect(Object *parent, const glm::vec2& position, ResID tex_anim, int frame_count, const glm::vec2& scale, 
    float frame_interval)
    :ObjectWorld(parent)
{
    setPosition(position);
    anim_ = new SpriteAnim(this, tex_anim, frame_count, AchorMode::CENTER, scale, frame_interval);
    anim_->setLoop(false);
}


void Effect::update(float dt)
{
    ObjectWorld::update(dt);

    if(anim_->getIsFinished())
    {
        setCanRemove(true);
        if(on_finished_) on_finished_();
    }
}