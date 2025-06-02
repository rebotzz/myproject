#ifndef __MOVE_CONTROL_H__
#define __MOVE_CONTROL_H__

#include "../core/object.h"
#include "../core/actor.h"

class MoveControl : public Object
{
protected:
    bool is_move_left_ = false;
    bool is_move_right_ = false;
    bool is_move_up_ = false;
    bool is_move_down_ = false;
public:
    MoveControl(Actor* parent):Object(parent) {}

    virtual void update(float) override;


    // getters and setters
    bool getMoveLeft() const { return is_move_left_; }
    bool getMoveRight() const { return is_move_right_; }    
    bool getMoveUp() const { return is_move_up_; }
    bool getMoveDown() const { return is_move_down_; }

};


#endif // __MOVE_CONTROL_H__
