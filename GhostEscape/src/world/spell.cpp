#include "spell.h"
#include "../core/scene.h"
#include "../core/actor.h"

Spell::Spell(Object *parent, float damage, const glm::vec2& target_position, CollideShape shape, ResID tex_id, 
    int frame_count, const glm::vec2& scale, float frame_interval, AchorMode mode)
    :ObjectWorld(parent), damage_(damage)
{
    setPosition(target_position);
    anim_ = new SpriteAnim(this, tex_id, frame_count, mode, scale, frame_interval, false);
    collide_box_ = new CollideBox(this, shape, anim_->getScaledSize(), glm::vec2(0), mode);
    collide_box_->setHitLayer(CollideLayer::Enemy); // 法术默认攻击敌人对象
    collide_box_->setOnCollideCallback([this](CollideBox * target_box)
    {
        auto target = target_box->getParent();
        dynamic_cast<Actor*>(target)->takeDamage(damage_);
    });
}

void Spell::update(float dt)
{
    ObjectWorld::update(dt);
    move(dt);
    checkAndRemove();
}

Spell* Spell::clone() const
{
    return new Spell(game_.getCurrentScene(), damage_, getPosition(), collide_box_->getCollideShape(), 
        anim_->getTexID(), anim_->getFrameCount(), anim_->getScale(), anim_->getFrameInterval(), anim_->getAchorMode());
}

void Spell::setHitCollideLayer(CollideLayer layer)
{
    collide_box_->setHitLayer(layer);
}

void Spell::setHurtCollideLayer(CollideLayer layer)
{
    collide_box_->setHurtLayer(layer);
}

void Spell::checkAndRemove()
{
    if(anim_ && anim_->getIsFinished())
    {
        setCanRemove(true);
    }
}

void Spell::setActive(bool flag)
{
    ObjectWorld::setActive(flag);
    collide_box_->setActive(flag);
}
