#include "spell.h"
#include "../core/scene.h"
#include "../enemy.h"

Spell::Spell(Object *parent, float damage, const glm::vec2& target, CollideShape shape, ResID tex_id, 
    int frame_count, float scale, float frame_interval, AchorMode mode)
    :damage_(damage)
{
    if(parent)
    {
        parent->addChild(this);
        setParent(parent);
    }
    setPosition(target);
    anim_ = SpriteAnim::createAndAddSpriteAnimChild(this, tex_id, frame_count, scale, frame_interval, false, glm::vec2(0.0f), mode);
    collide_box_ = CollideBox::createAndAddCollideBoxChild(this, shape, anim_->getSize(), glm::vec2(0), mode);
}

void Spell::update(float dt)
{
    ObjectWorld::update(dt);
    updateCollide();
    if(anim_ && anim_->getIsFinished())
    {
        setCanRemove(true);
    }
}

void Spell::updateCollide()
{
    auto objects = dynamic_cast<Scene*>(parent_)->getWorldObjects();
    for(auto obj : objects)
    {
        if(obj->getObjectType() != ObjectType::Enemy) continue;
        auto enemy = dynamic_cast<Enemy*>(obj);
        if(collide_box_->checkCollision(enemy->getCollideBox()))
        {
            enemy->takeDamage(damage_);
        }
    }
}
