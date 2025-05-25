#include "effect.h"
#include "../core/scene.h"

Effect::Effect(Object *parent, const glm::vec2& position, ResID tex_anim, int total_frame_count, float scale, 
    float frame_interval):ObjectWorld(parent)
{
    setPosition(position);
    anim_ = SpriteAnim::createAndAddSpriteAnimChild(this, tex_anim, total_frame_count, scale, frame_interval, false);

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