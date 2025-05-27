#include "player.h"
#include "core/scene.h"
#include "affiliate/sprite_anim.h"
#include "affiliate/collide_box.h"
#include "core/status.h"
#include "scene_main.h"
#include "enemy.h"
#include "hud/ui_player_status.h"
#include "raw/effect.h"

#include "weapon_thunder.h"

Player::Player(Scene* parent, const glm::vec2& position)
{
    setPosition(position);
    if(parent) 
    {
        parent->safeAddChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Player);
    // 初始化动画
    anim_move_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostMove, 8, 3.0f);
    anim_idle_ = SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostIdle, 8, 3.0f);
    anim_idle_->setRenderPosition(game_.getCurrentScene()->worldToScreen(position));  
    anim_move_->setRenderPosition(game_.getCurrentScene()->worldToScreen(position));    
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
    status_ = Status::createAndAddStatusChild(this, 200.0f, 300.0f, 0.03f, 1.5f);
    status_->setOnHurtCallback([this](){ game_.playSound(ResID::Sound_FemaleScream0289290); });
    // 武器, 武器挂载到玩家，跟随玩家；武器生成的法术挂载到场景，不随玩家移动
    weapon_thunder_ = WeaponThunder::createAndAddWeaponThunderChild(this, 50.0f, 1.5f, 100.f);
    // 出生、死亡特效，挂载到场景
    effect_dead_ = new Effect(parent, position, ResID::Tex_1843, 11, 3.0f, 0.07f);
    effect_born_ = new Effect(parent, position, ResID::Tex_18432, 11, 3.0f, 0.07f);
    effect_dead_->setActive(false);
    effect_born_->setOnFinished([this](){ setActive(true); });
    setActive(false);   // 出生特效结束才显示玩家

    // UI界面 挂载到场景
    UIPlayerStatus::createAndAddUIPlayerStatusChild(parent);
    game_.playSound(ResID::Sound_SillyGhostSound242342);
}
Player::~Player()
{

}

Player *Player::createAndAddPlayerChild(Scene *parent, const glm::vec2& position)
{
    auto player = new Player(parent, position);
    return player;
}

bool Player::handleEvent(const SDL_Event& event)
{
    if(Actor::handleEvent(event)) return true;
    bool event_handled = false;
    // 技能释放
    switch(event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_UP:
        if(!weapon_thunder_->canAttack()) break;
        // 世界 = 渲染 + 相机
        // 渲染坐标 = 世界 - 相机
        glm::vec2 cursor_pos;
        game_.getMouseState(cursor_pos);    // 不能直接SDL_获取，因为屏幕缩放会导致鼠标位置错位
        auto target = cursor_pos + dynamic_cast<Scene*>(parent_)->getCameraPosition();
        weapon_thunder_->attack(target);
        event_handled = true;
        break;
    }
    return event_handled;
}

void Player::update(float dt) 
{
    Actor::update(dt);

    if(status_->getIsDead())
    {
        effect_dead_->setActive(true);
        effect_dead_->setPosition(getPosition());
        setActive(false);
        return;
    }

    velocity_ *= 0.9f;
    updateKeyboardControl();
    move(dt);
    updateSpriteAnim();
    syncCamera(dt);
}

void Player::render() 
{
    Actor::render();
}

void Player::updateKeyboardControl()
{
    auto keystats = SDL_GetKeyboardState(nullptr);
    auto direction = glm::normalize(glm::vec2(keystats[SDL_SCANCODE_D] - keystats[SDL_SCANCODE_A], 
        keystats[SDL_SCANCODE_S] - keystats[SDL_SCANCODE_W]));
    if(glm::length(direction) > 0.1)
        velocity_ = direction * max_speed_;
}

void Player::move(float dt)
{
    // 限制玩家位置
    auto position = glm::clamp(world_position_ + velocity_ * dt, anim_move_->getSize() * 0.2f, 
        dynamic_cast<Scene*>(parent_)->getWorldSize() - anim_move_->getSize() * 0.2f);
    setPosition(position);
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
        anim_move_->setRenderPosition(anim_idle_->getRenderPosition());
    }
    else
    {
        anim_move_->setActive(true);
        anim_idle_->setActive(false);
        anim_idle_->syncFrameTime(anim_move_);
        anim_idle_->setRenderPosition(anim_move_->getRenderPosition());
    }

    if(status_->getIsInvincible() && (static_cast<int>(status_->getInvincibleProgress() * 10.0f) % 3 < 1))
    {
        anim_idle_->setShowing(!anim_idle_->getShowing());
        anim_move_->setShowing(!anim_move_->getShowing());
    }
    if(!status_->getIsInvincible())
    {
        anim_idle_->setShowing(true);
        anim_move_->setShowing(true);
    }

}

void Player::syncCamera(float dt)
{
    auto camera_pos = getPosition() - game_.getScreenSize() * 0.5f; // 不需要 + anim_move_->getSize() * 0.5f，玩家默认中心锚点
    game_.getCurrentScene()->cameraFollow(dt, camera_pos);
}
