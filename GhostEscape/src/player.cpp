#include "player.h"
#include "core/scene.h"
#include "core/sprite_anim.h"
#include "core/collide_box.h"
#include "core/status.h"
#include "scene_main.h"
#include "enemy.h"
#include "ui/ui_player_status.h"

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
    // 初始化碰撞箱体
    collide_box_ = CollideBox::createAndAddCollideBoxChild(this, CollideShape::Circle, anim_move_->getSize() * 0.5f);
    // 初始化状态
    status_ = Status::createAndAddStatusChild(this, 200.0f, 300.0f, 0.1f, 1.5f);
    // 武器, 武器挂载到玩家，跟随玩家；武器生成的法术挂载到场景，不随玩家移动
    weapon_thunder_ = WeaponThunder::createAndAddWeaponThunderChild(this, 50.0f, 2.0f);

    // UI界面 挂载到场景
    UIPlayerStatus::createAndAddUIPlayerStatusChild(parent);
}
Player::~Player()
{

}

Player *Player::createAndAddPlayerChild(Scene *parent, const glm::vec2& position)
{
    auto player = new Player(parent, position);
    return player;
}

void Player::handleEvent(const SDL_Event& event)
{
    Actor::handleEvent(event);

    // 技能释放
    switch(event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_UP:
        if(!weapon_thunder_->canAttack()) break;
        // 世界 = 渲染 + 相机
        // 渲染坐标 = 世界 - 相机
        glm::vec2 cursor_pos = glm::vec2(event.motion.x, event.motion.y);
        auto target = cursor_pos + dynamic_cast<Scene*>(parent_)->getCameraPosition();
        weapon_thunder_->attack(target);
        break;
    }
}

void Player::update(float dt) 
{
    Actor::update(dt);

    velocity_ *= 0.9f;
    updateKeyboardControl();
    updateMotion(dt);
    updateSpriteAnim();

}

void Player::render() 
{
    if(status_->getIsInvincible() && (static_cast<int>(status_->getInvincibleProgress() * 10.0f) % 5 < 2)) return;
    Actor::render();

    // game_.renderBoundary(render_position_, render_position_ + glm::vec2(5), 5);
}

void Player::updateKeyboardControl()
{
    auto keystats = SDL_GetKeyboardState(nullptr);
    auto direction = glm::normalize(glm::vec2(keystats[SDL_SCANCODE_D] - keystats[SDL_SCANCODE_A], 
        keystats[SDL_SCANCODE_S] - keystats[SDL_SCANCODE_W]));
    if(glm::length(direction) > 0.1)
        velocity_ = direction * speed_;
}

void Player::updateMotion(float dt)
{
    world_position_ += velocity_ * dt;
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
    }
    else
    {
        anim_move_->setActive(true);
        anim_idle_->setActive(false);
        anim_idle_->syncFrameTime(anim_move_);
    }
}

