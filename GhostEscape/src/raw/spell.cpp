#include "spell.h"
#include "../core/scene.h"
#include "../core/actor.h"

Spell::Spell(Object *parent, float damage, const glm::vec2& target_position, CollideShape shape, ResID tex_id, 
    int frame_count, float scale, float frame_interval, AchorMode mode)
    :ObjectWorld(parent), damage_(damage)
{
    setPosition(target_position);
    anim_ = SpriteAnim::createAndAddSpriteAnimChild(this, tex_id, frame_count, scale, frame_interval, false, glm::vec2(0.0f), mode);
    collide_box_ = new CollideBox(this, shape, anim_->getSize(), glm::vec2(0), mode);
    collide_box_->setHitLayer(CollideLayer::Enemy); // 法术默认攻击对象
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
    updateCollide();
    checkAndRemove();
}

void Spell::updateCollide()
{
    // auto objects = dynamic_cast<Scene*>(parent_)->getWorldObjects();
    // for(auto obj : objects)
    // {
    //     if(obj->getObjectType() != attack_target_type_) continue;
    //     auto target = dynamic_cast<Actor*>(obj);
    //     if(collide_box_->checkCollision(target->getCollideBox()))
    //     {
    //         target->takeDamage(damage_);
    //         collide_box_->processCollide();
    //     }
    // }
}

void Spell::checkAndRemove()
{
    if(anim_ && anim_->getIsFinished())
    {
        setCanRemove(true);
    }
}