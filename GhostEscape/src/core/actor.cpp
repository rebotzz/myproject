#include "actor.h"
#include "../affiliate/affiliate_bar.h"
#include "../affiliate/collide_box.h"
#include "../raw/move_control.h"


void Actor::update(float dt)
{
    velocity_ *= 0.9f;
    ObjectWorld::update(dt);    // 更新速度等
    move(dt);
    updateHealthBar();
    if(is_sync_camera) syncCamera(dt);
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
        margin_bottom_right = collide_box_->getOffset() + collide_box_->getScaledSize();
    }
    auto limited_position = glm::clamp(position, glm::vec2(0) - margin_top_left, game_.getCurrentScene()->getWorldSize() - margin_bottom_right);
    setPosition(limited_position);
}

void Actor::updateHealthBar()
{
    if(!status_ || !health_bar_) return;
    health_bar_->setPercentage(status_->getHP() / status_->getMaxHP());
}

void Actor::syncCamera(float dt)
{
    auto camera_pos = getPosition() - game_.getScreenSize() * 0.5f; // 不需要+anim_move_->getSize() * 0.5f，玩家默认中心锚点
    game_.getCurrentScene()->cameraFollow(dt, camera_pos);
}

void Actor::removeControl()
{
    if(move_control_)
    {
        move_control_->setCanRemove(true);
        move_control_ = nullptr;
    }
}
