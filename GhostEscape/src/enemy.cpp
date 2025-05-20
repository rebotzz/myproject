#include "enemy.h"
#include "core/sprite_anim.h"
#include "core/collide_box.h"
#include "scene_main.h"
#include "player.h"

void Enemy::init()
{
    anim_move_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostSheet, 4, 3.0f);
    anim_hurt_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostHurtSheet, 4, 3.0f);
    anim_dead_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostDeadSheet, 8, 3.0f);
    anim_hurt_->setActive(false);
    anim_dead_->setActive(false);
    CollideBox::createAndAddCollideBoxChild(this, CollideShape::Circle, anim_move_->getSize() * 0.8f);
    setSpeed(100.f);
    setObjectType(ObjectType::Enemy);
}


Enemy::Enemy(Object *parent, const glm::vec2 &position)
{
    setWorldPosition(position);
    if(parent)
    {
        setParent(parent);
        parent->addChild(this);
    }
    init();
}

Enemy::~Enemy()
{
}

Enemy *Enemy::createAndAddEnemyChild(Object *parent, const glm::vec2 &position)
{
    auto enemy = new Enemy(parent, position);
    return enemy;
}

void Enemy::update(float dt)
{
    Actor::update(dt);
    velocity_ *= 0.9f;
    updateMotion(dt);
    updateAnim();
}

void Enemy::updateMotion(float dt)
{
    auto player = dynamic_cast<SceneMain*>(parent_)->getPlayer();
    auto player_pos = player->getPosition();
    float distance = glm::length(getPosition() - player_pos);
    if(distance < 0.1) return;
    velocity_ = glm::normalize(player_pos - getPosition()) * speed_;
    world_position_ += velocity_ * dt;
}

void Enemy::updateAnim()
{
    // TODO: 受击，死亡切换动画：stat类
    // 更换current_anim_
}