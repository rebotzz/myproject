#include "actor.h"
#include "../affiliate/affiliate_bar.h"
#include "../affiliate/collide_box.h"
#include "../raw/move_control.h"


void Actor::update(float dt)
{
    ObjectWorld::update(dt);
    move(dt);
    updateHealthBar();
}

void Actor::setMoveControl(MoveControl *val)
{   
    if(move_control_)
    {
        move_control_->setCanRemove(true);
    }
    move_control_ = val;
}

void Actor::move(float dt)
{
    // 移动
    auto position = world_position_ + velocity_ * dt;
    // 限制角色位置 根据碰撞盒大小来限制移动区域
    glm::vec2 margin_top_left = glm::vec2(0);
    glm::vec2 margin_bottom_right = glm::vec2(0);
    if (collide_box_){
        margin_top_left = collide_box_->getOffset();
        margin_bottom_right = collide_box_->getOffset() + collide_box_->getSize();
    }
    auto limited_position = glm::clamp(position, glm::vec2(0) - margin_top_left, game_.getCurrentScene()->getWorldSize() - margin_bottom_right);
    setPosition(limited_position);
}

void Actor::updateHealthBar()
{
    if(!status_ || !health_bar_) return;
    health_bar_->setPercentage(status_->getHP() / status_->getMaxHP());
}

void Actor::removeControl()
{
    if(move_control_)
    {
        move_control_->setCanRemove(true);
        move_control_ = nullptr;
    }
}
