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
    setMaxSpeed(130.f);
    setObjectType(ObjectType::Enemy);
    // 初始化动画
    anim_move_ = new SpriteAnim(this, ResID::Tex_GhostSheet, 4, AchorMode::CENTER, glm::vec2(1.5));
    anim_hurt_ = new SpriteAnim(this, ResID::Tex_GhostHurtSheet, 4, AchorMode::CENTER, glm::vec2(1.5));
    anim_dead_ = new SpriteAnim(this, ResID::Tex_GhostDeadSheet, 8, AchorMode::CENTER, glm::vec2(1.5));
    anim_dead_->setLoop(false);
    anim_hurt_->setActive(false);
    anim_dead_->setActive(false);
    current_anim_ = anim_move_;
    // 碰撞盒子         
    collide_box_ = new CollideBox(this, CollideShape::Circle, anim_move_->getScaledSize() * 0.8f);  
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
    // 武器,敌人残血时开启远程攻击
    weapon_ = new Weapon(this, 10.f, 5.f, 0.0f);
    weapon_->setSoundID(ResID::Sound_XsLaser);
    weapon_->setActive(false);
    weapon_->setAutoAttack(true);
    static SpellBullet* spell_bullet = nullptr;
    if(!spell_bullet)
    {
        // todo: 这个原型怎么删除呢？如果每个敌人都生成一个原型，太浪费了
        spell_bullet = new SpellBullet(nullptr, 10.f, world_position_, ResID::Tex_Laser3, 1, glm::vec2(0.5f));
        spell_bullet->setMaxSpeed(500.f);
        spell_bullet->setDirection(glm::vec2(1,0));
        spell_bullet->setHitCollideLayer(CollideLayer::Player);
        spell_bullet->setActive(false);
    }
    weapon_->setSpellProtype(spell_bullet);
    weapon_->setAutoAttackCallback([&]()
    {
        // 瞄准玩家，发射子弹
        if(!target_ || target_->getIsDead() || !target_->getIsActive()) return;
        auto target_pos = target_->getPosition();
        auto direction = glm::length(target_pos - getPosition()) > 0.1f ? glm::normalize(target_pos - getPosition()) : glm::vec2(1, 0);
        auto spell = weapon_->attack(getPosition());
        dynamic_cast<SpellBullet*>(spell)->setDirection(direction);
    });

    // 血条  最后添加，最上层最后绘制
    health_bar_ = new AffilateBar(this, glm::vec2(anim_move_->getScaledSize().x * 0.8f, 10.0f), glm::vec2(0.0, anim_move_->getScaledSize().y / 2.0f));
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt)
{
    Actor::update(dt);
    updateVelocity();

    updateState();
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