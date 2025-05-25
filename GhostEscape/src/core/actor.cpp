#include "actor.h"

void Actor::move(float dt)
{
    // 限制角色位置
    auto position = glm::clamp(world_position_ + velocity_ * dt, glm::vec2(0), 
        dynamic_cast<Scene*>(parent_)->getWorldSize());
    setPosition(position);
}