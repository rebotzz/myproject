#include "player.h"
#include "core/scene.h"
#include "core/sprite_anim.h"

Player::Player()
{
    SpriteAnim::createAndAddSpriteAnimChild(this, ResID::Tex_GhostMove, 8, 3.0f);
}
Player::~Player()
{

}

Player *Player::createAndAddPlayerChild(Object *parent, glm::vec2 position)
{
    auto player = new Player();
    player->setWorldPosition(position);
    if(parent) 
    {
        parent->addChild(player);
        player->parent_ = parent;
    }
    return player;
}

void Player::handleEvent(const SDL_Event& event)
{
    Actor::handleEvent(event);
}
void Player::update(float dt) 
{
    Actor::update(dt);

    velocity_ *= 0.9;
    updateKeyboardControl();
    updateMotion(dt);

    // 玩家必须挂载到场景，不然没有意义。
    updateRenderPosition(dynamic_cast<Scene*>(parent_)->getCameraPosition());
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
        velocity_ = direction * speed_;
}

void Player::updateMotion(float dt)
{
    world_position_ += velocity_ * dt;
}
