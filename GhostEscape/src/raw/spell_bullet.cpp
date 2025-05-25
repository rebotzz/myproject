#include "spell_bullet.h"
#include "../core/scene.h"

SpellBullet::SpellBullet(Object *parent, float damage, const glm::vec2 &init_position, ResID tex_id, 
    int frame_count, float scale, float frame_interval)
    :Spell(parent, damage, init_position, CollideShape::Circle, tex_id, frame_count, scale, frame_interval)
{
    anim_->setLoop(true);
    collide_box_->setOnCollideCallback([this]() { setActive(false); });
    collide_box_->setHitLayer(CollideLayer::Player);
    game_.playSound(ResID::Sound_XsLaser);
}

void SpellBullet::move(float dt)
{
    setPosition(getPosition() + direction_ * max_speed_ * dt);
}

void SpellBullet::checkAndRemove()
{
    // 超出屏幕失效
    if(dynamic_cast<Scene*>(parent_)->checkBeyoundScreen(getPosition(), anim_->getSize()))
    {
        setCanRemove(true);
    }
}
