#include "move_control.h"


void MoveControl::update(float)
{
    glm::vec2 dir = {is_move_right_ - is_move_left_, is_move_down_ - is_move_up_};
    // dynamic_cast<Actor*>(parent_)->setVelocity(dynamic_cast<Actor*>(parent_)->getVelocity() * 0.9f);
    if(glm::length(dir) > 0.1f) 
    {
        dynamic_cast<Actor*>(parent_)->setVelocity(glm::normalize(dir) * dynamic_cast<Actor*>(parent_)->getMaxSpeed());
    }
}