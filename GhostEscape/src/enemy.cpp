#include "enemy.h"
#include "core/sprite_anim.h"
#include "core/collide_box.h"
#include "core/status.h"
#include "scene_main.h"
#include "player.h"
#include "ui/ui_enemy_hp_bar.h"

Enemy::Enemy(Object *parent, const glm::vec2 &position)
{
    // 基础设置
    if(parent)
    {
        setParent(parent);
        parent->safeAddChild(this);
    }
    setPosition(position);
    setSpeed(100.f);
    setObjectType(ObjectType::Enemy);
    // 初始化动画
    anim_move_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostSheet, 4, 3.0f);
    anim_hurt_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostHurtSheet, 4, 3.0f);
    anim_dead_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostDeadSheet, 8, 3.0f, 0.1f, false);
    anim_hurt_->setActive(false);
    anim_dead_->setActive(false);
    current_anim_ = anim_move_;
    // 碰撞盒子
    collide_box_ = CollideBox::createAndAddCollideBoxChild(this, CollideShape::Circle, anim_move_->getSize() * 0.8f);
    // 状态
    status_ = Status::createAndAddStatusChild(this, 120.0f, 0.0f, 100.0f, 1.4f);

    // 血条UI  最后添加，最上层最后绘制
    UIEnemyHPBar::createAndAddUIHPBarChild(this, glm::vec2(anim_move_->getSize().x * 0.8f, 10.0f), glm::vec2(0.0, anim_move_->getSize().y / 2.0f));
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
    updateCollide();
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
    auto old_anim = current_anim_;
    if(status_->getIsDead())
    {
        current_anim_ = anim_dead_;
    }
    else if(status_->getIsInvincible())
    {
        current_anim_ = anim_hurt_;
    }
    else
    {
        current_anim_ = anim_move_;
    }

    if(current_anim_ != old_anim)
    {
        old_anim->setActive(false);
        current_anim_->restart();
        current_anim_->setActive(true);
        // 同步动画位置
        current_anim_->setRenderPosition(old_anim->getRenderPosition());
    }
    
    // 死亡动画结束，场景需要移除敌人
    if(anim_dead_->getIsFinished()) 
    {
        setCanRemove(true);
        setActive(false);
    }
}

void Enemy::updateCollide()
{
    auto player = dynamic_cast<SceneMain*>(parent_)->getPlayer();
    if(!player->getStatus()->getIsInvincible() && collide_box_->checkCollision(player->getCollideBox()))
    {
        player->getStatus()->takeDamage(status_->getDamage());
    }
}

void Enemy::takeDamage(float damage)
{
    status_->takeDamage(damage);
}
