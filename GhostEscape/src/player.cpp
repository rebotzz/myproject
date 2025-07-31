#include "player.h"
#include "core/scene.h"
#include "affiliate/sprite_anim.h"
#include "affiliate/collide_box.h"
#include "raw/status.h"
#include "scene_main.h"
#include "enemy.h"
#include "world/effect.h"
#include "world/spell.h"
#include "raw/weapon.h"
#include "raw/move_control_keyboardAWSD.h"
#include "raw/timer.h"

Player::Player(Scene* parent, const glm::vec2& position):Actor(parent)
{
    setEnableSyncCamera(true);
    setPosition(position);
    setObjectType(ObjectType::Player);
    // 初始化动画
    anim_move_ = new SpriteAnim(this, ResID::Tex_GhostMove, 8, AchorMode::CENTER, glm::vec2(2.0));
    anim_idle_ = new SpriteAnim(this, ResID::Tex_GhostIdle, 8, AchorMode::CENTER, glm::vec2(2.0));
    // anim_idle_->setRenderPosition(game_.getCurrentScene()->worldToScreen(position));     // TODO
    // anim_move_->setRenderPosition(game_.getCurrentScene()->worldToScreen(position));    
    // 初始化碰撞箱体
    collide_box_ = new CollideBox(this, CollideShape::Circle, anim_move_->getSize() * 0.5f);
    collide_box_->setHurtLayer(CollideLayer::Player);
    collide_box_->setOnCollideCallback([this](CollideBox * target_box)
    {
        auto target_type = target_box->getParent()->getObjectType();
        if(target_type == ObjectType::Enemy)
        {
            takeDamage(dynamic_cast<Enemy*>(target_box->getParent())->getStatus()->getDamage());
        }
        else
        {
            takeDamage(dynamic_cast<Spell*>(target_box->getParent())->getDamage());
        }
    });
    // 初始化状态
    status_ = Status::createAndAddStatusChild(this, 200.0f, 400.0f, 50.f, 1.5f);
    status_->setOnHurtCallback([this](){ game_.playSound(ResID::Sound_FemaleScream0289290); });
    // 武器, 武器挂载到玩家，跟随玩家；武器生成的法术挂载到场景，不随玩家移动
    weapon_thunder_ = new Weapon(this, 50.0f, 1.5f, 100.f);
    weapon_thunder_->setSoundID(ResID::Sound_BigThunder);
    auto spell_thunder = new Spell(game_.getCurrentScene(), weapon_thunder_->getDamage(), position, CollideShape::Circle, 
        ResID::Tex_ThunderstrikeWBlur, 13, glm::vec2(3.8f));
    spell_thunder->getCollideBox()->setHitLayer(CollideLayer::Enemy);
    spell_thunder->setActive(false);
    weapon_thunder_->setSpellProtype(spell_thunder);

    weapon_fire_ = new Weapon(this, 100.0f, 2.0f, 200.f);
    weapon_fire_->setSoundID(ResID::Sound_FireMagic);
    auto spell_fire = new Spell(game_.getCurrentScene(), weapon_fire_->getDamage(), position, CollideShape::Circle, 
        ResID::Tex_FireExplosion, 18, glm::vec2(7.5));
    spell_fire->getCollideBox()->setHitLayer(CollideLayer::Enemy);
    spell_fire->setActive(false);
    weapon_fire_->setSpellProtype(spell_fire);
    weapon_fire_->setAttackKey(SDL_BUTTON_RIGHT);

    // 出生、死亡特效，挂载到场景
    effect_dead_ = new Effect(parent, position, ResID::Tex_1843, 11, glm::vec2(1.5f), 0.07f);
    effect_born_ = new Effect(parent, position, ResID::Tex_18432, 11, glm::vec2(1.5f), 0.07f);
    effect_dead_->setActive(false);
    effect_born_->setOnFinished([this](){ setActive(true); });
    setActive(false);   // 出生特效结束才显示玩家

    game_.playSound(ResID::Sound_SillyGhostSound242342);

    // 角色控制
    setMoveControl(new MoveControlKeyboardAWSD(this));

    // 自动逃跑
    decide_timer = new Timer(this, 0.5f);
}

Player::~Player()
{

}

bool Player::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_EVENT_KEY_UP)
    {
        if(event.key.scancode == SDL_SCANCODE_F1)
        {
            setMoveControl(new MoveControlKeyboardAWSD(this));
            return true;
        }
        else if(event.key.scancode == SDL_SCANCODE_F2)
        {
            setMoveControl(new MoveControlKeyboardArrow(this));
            return true;
        }
        else if(event.key.scancode == SDL_SCANCODE_Q)
        {
            if(getMoveControl())
            {
                findNearestEnemy();
                switchControlWithEnemy(nearest_enemy);
                if(control_enemy)
                {
                    setEnableSyncCamera(false);
                    control_enemy->setMaxSpeed(300.f);
                    control_enemy->setEnableSyncCamera(true);
                }
            }
            else
            {
                if(checkEnemyInScene(control_enemy))
                {
                    control_enemy->removeControl();
                    control_enemy->setEnableSyncCamera(false);
                    control_enemy->setMaxSpeed(150.f);
                }
                control_enemy = nullptr;
                setEnableSyncCamera(true);
                setMoveControl(new MoveControlKeyboardAWSD(this));
            }
        }
    } 

    return Actor::handleEvent(event);
}

void Player::update(float dt) 
{
    Actor::update(dt);
    if(!move_control_)
    {
        autoEscape();
    }
    if(status_->getIsDead())
    {
        effect_dead_->setActive(true);
        effect_dead_->setPosition(getPosition());
        setActive(false);
        return;
    }

    updateSpriteAnim();
}

void Player::render() 
{
    Actor::render();
}

void Player::updateSpriteAnim()
{
    // 动画左右方向
    if(velocity_.x > 0)
    {
        anim_move_->setFlip(false);
        anim_idle_->setFlip(false);
    }
    else if(velocity_.x < 0)
    {
        anim_move_->setFlip(true);
        anim_idle_->setFlip(true);
    }

    // 切换动画
    if(glm::length(velocity_) < 0.1) 
    {
        anim_move_->setActive(false);
        anim_idle_->setActive(true);
        anim_move_->syncFrameTime(anim_idle_);
        // anim_move_->setRenderPosition(anim_idle_->getRenderPosition());  // todo
    }
    else
    {
        anim_move_->setActive(true);
        anim_idle_->setActive(false);
        anim_idle_->syncFrameTime(anim_move_);
        // anim_idle_->setRenderPosition(anim_move_->getRenderPosition());
    }

    // TODO
    // if(status_->getIsInvincible() && (static_cast<int>(status_->getInvincibleProgress() * 10.0f) % 3 < 1))
    // {
    //     anim_idle_->setActive(!anim_idle_->getIsActive());
    //     anim_move_->setActive(!anim_move_->getIsActive());
    // }
    // if(!status_->getIsInvincible())
    // {
    //     anim_idle_->setActive(true);
    //     anim_move_->setActive(true);
    // }

}

void Player::autoEscape()
{
    // 玩家自动逃跑
    // 控制决策频率，找到场景中最近敌人
    if(decide_timer->timeOut())
    {
        findNearestEnemy();
    }
    // 变更速度方向，逃避
    if(!nearest_enemy) return;
    auto dir = getPosition() - nearest_enemy->getPosition();
    if(glm::length(dir) > 0.1f)
    {
        setVelocity(glm::normalize(dir) * getMaxSpeed());
    }
    // 地图死角判断
    auto scene = dynamic_cast<Scene*>(parent_);
    auto corners = {glm::vec2(0.0f), glm::vec2(0.0f, scene->getWorldSize().y), 
        glm::vec2(scene->getWorldSize().x, 0.0f), scene->getWorldSize()};
    bool in_corner = false;
    float distance = -1.0f;
    for(auto& corner : corners)
    {
        distance = glm::length(getPosition() - corner);
        if(distance > 0.1f && distance < (collide_box_->getSize().x + collide_box_->getSize().y) * 0.7)
        {
            setVelocity(glm::normalize(getPosition() - corner) * getMaxSpeed());
            break;
        }
    }
    if(in_corner)
    {
        decide_timer->setInterval(decide_interval * 3.0f);
    }
    else if(distance > (collide_box_->getSize().x + collide_box_->getSize().y) * 10.0f)
    {
        decide_timer->setInterval(decide_interval);
    }

}

void Player::switchControlWithEnemy(Actor *enemy)
{
    if(!enemy) return;
    enemy->setMoveControl(new MoveControlKeyboardAWSD(enemy));
    control_enemy = enemy;
    removeControl();
}

Actor *Player::findNearestEnemy()
{
    // 更新敌人，防止失效
    auto scene = dynamic_cast<Scene*>(parent_);
    auto& scene_objects = scene->getWorldObjects();
    if(!checkEnemyInScene(nearest_enemy))
    {
        nearest_enemy = nullptr;
    }
    for(Object* obj : scene_objects)
    {
        if(obj->getObjectType() == ObjectType::Enemy)
        {
            auto enemy = dynamic_cast<Actor*>(obj);
            if(nullptr == nearest_enemy || 
                glm::length(enemy->getPosition() - getPosition()) < glm::length(nearest_enemy->getPosition() - getPosition()))
            {
                nearest_enemy = enemy;
            }
        }
    }

    return nearest_enemy;
}

bool Player::checkEnemyInScene(Actor *enemy)
{
    auto scene = dynamic_cast<Scene*>(parent_);
    auto& scene_objects = scene->getWorldObjects();
    return std::find(scene_objects.begin(), scene_objects.end(), enemy) != scene_objects.end();
}

