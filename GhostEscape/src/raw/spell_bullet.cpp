#include "spell_bullet.h"
#include "../core/scene.h"
#include "../core/actor.h"

SpellBullet::SpellBullet(Object *parent, float damage, const glm::vec2 &init_position, ResID tex_id, 
    int frame_count, float scale, float frame_interval)
    :Spell(parent, damage, init_position, CollideShape::Circle, tex_id, frame_count, scale, frame_interval)
{
    anim_->setLoop(true);
    collide_box_->setOnCollideCallback([this](CollideBox * target_box) 
    {
        auto target = target_box->getParent();
        dynamic_cast<Actor*>(target)->takeDamage(damage_);
        setActive(false); 
        setCanRemove(true); // debug:这里应该是移除，而不仅仅是不激活
    });
    collide_box_->setHitLayer(CollideLayer::Player);
    collide_box_->setSize(collide_box_->getSize() * 0.5f);
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
