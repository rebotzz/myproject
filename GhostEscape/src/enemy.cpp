#include "enemy.h"
#include "affiliate/sprite_anim.h"
#include "affiliate/collide_box.h"
#include "raw/status.h"
#include "scene_main.h"
#include "affiliate/affiliate_bar.h"
#include "raw/weapon.h"
#include "world/spell_bullet.h"
#include "player.h"

Enemy::Enemy(Object *parent, const glm::vec2 &position):Actor(parent)
{
    // 基础设置
    setPosition(position);
    setMaxSpeed(100.f);
    setObjectType(ObjectType::Enemy);
    // 初始化动画
    anim_move_ = new SpriteAnim(this, ResID::Tex_GhostSheet, 4, AchorMode::CENTER, glm::vec2(2));
    anim_hurt_ = new SpriteAnim(this, ResID::Tex_GhostHurtSheet, 4, AchorMode::CENTER, glm::vec2(2));
    anim_dead_ = new SpriteAnim(this, ResID::Tex_GhostDeadSheet, 8, AchorMode::CENTER, glm::vec2(2));
    anim_dead_->setLoop(false);
    anim_hurt_->setActive(false);
    anim_dead_->setActive(false);
    current_anim_ = anim_move_;
    // 碰撞盒子
    collide_box_ = new CollideBox(this, CollideShape::Circle, anim_move_->getSize() * 0.8f);
    collide_box_->setHitLayer(CollideLayer::Player);
    collide_box_->setHurtLayer(CollideLayer::Enemy);
    collide_box_->setOnCollideCallback([this](CollideBox * target_box)
    {
        if(target_box->getHitLayer() == CollideLayer::Enemy)
        {
            takeDamage(dynamic_cast<Spell*>(target_box->getParent())->getDamage());
        }
    });
    // 状态
    status_ = Status::createAndAddStatusChild(this, 120.0f, 0.0f, 100.0f, 1.4f);
    status_->setOnHurtCallback([this](){ game_.playSound(ResID::Sound_HitFlesh02266309); });
    // 武器
    weapon_ = new Weapon(this, 10.f, 5.f);
    weapon_->setAttackCallback([&]()
    {
        // 发射子弹
        if(!target_ || target_->getIsDead() || !target_->getIsActive()) return;
        auto target_pos = target_->getPosition();
        auto direction = glm::normalize(target_pos - getPosition());
        if(glm::length(direction) > 2.0f) direction = glm::vec2(1, 0);
        auto bullet = new SpellBullet(parent_, 10.f, world_position_, ResID::Tex_Laser3, 1, glm::vec2(0.5f));
        bullet->setDirection(direction);
        bullet->setMaxSpeed(500.f);
        float deg = glm::degrees(std::atan2(direction.y, direction.x)); // glm::angle的角度没有方向
        bullet->getSpriteAnim()->setAngle(deg);
        bullet->setHitCollideLayer(CollideLayer::Player);
    });
    weapon_->setActive(false);

    // 血条  最后添加，最上层最后绘制
    health_bar_ = new AffilateBar(this, glm::vec2(anim_move_->getSize().x * 0.8f, 10.0f), glm::vec2(0.0, anim_move_->getSize().y / 2.0f));
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
    velocity_ *= 0.9f;
    Actor::update(dt);
    updateVelocity();

    updateState();
    attack();
    checkAndRemove();
}

void Enemy::updateVelocity()
{
    if(move_control_) return;   // 手动控制

    if(!target_ || !target_->getIsActive()) return;
    if(current_state_ != EnemyState::NORMAL) return;
    auto target_pos = target_->getPosition();
    float distance = glm::length(getPosition() - target_pos);
    if(distance < 0.1) return;
    velocity_ = glm::normalize(target_pos - getPosition()) * max_speed_;
}

void Enemy::updateState()
{
    // 更换current_anim_
    auto old_state = current_state_;
    auto old_anim = current_anim_;
    if(status_->getIsDead()) current_state_ = EnemyState::DEAD;
    else if(status_->getIsInvincible()) current_state_ = EnemyState::HURT;
    else  current_state_ = EnemyState::NORMAL;

    if(old_state != current_state_)
    {
        switch(current_state_)
        {
            case EnemyState::NORMAL: current_anim_ = anim_move_; break;
            case EnemyState::HURT: current_anim_ = anim_hurt_; break;
            case EnemyState::DEAD: current_anim_ = anim_dead_; break;
        }
        if(current_anim_ != old_anim)
        {
            old_anim->setActive(false);
            current_anim_->restart();
            current_anim_->setActive(true);
            current_anim_->setRenderPosition(old_anim->getRenderPosition());    // 同步动画位置
        }
    }

    // 血量较低时:增加移速,发射子弹
    float hp_percentage = status_->getHP() / status_->getMaxHP();
    if(hp_percentage < 0.6f && hp_percentage > 0.4f)
    {
        setMaxSpeed(200.f);
    }
    else if(hp_percentage < 0.4f)
    {
        weapon_->setActive(true);
    }
}

void Enemy::checkAndRemove()
{
    // 死亡动画结束，场景需要移除敌人
    if(anim_dead_->getIsFinished()) 
    {
        setCanRemove(true);
        dynamic_cast<SceneMain*>(parent_)->getPlayer()->addScore(10);
    }
}

void Enemy::attack()
{
    if(weapon_->canAttack())
        weapon_->attack(getPosition());
}
