#ifndef _MOVE_CONTROL_KEYBOARD_H_
#define _MOVE_CONTROL_KEYBOARD_H_

#include "move_control.h"


class MoveControlKeyboardAWSD : public MoveControl
{
public:
    MoveControlKeyboardAWSD(Actor* parent):MoveControl(parent) {}

    virtual bool handleEvent(const SDL_Event& event) override;
};


class MoveControlKeyboardArrow : public MoveControl
{
public:
    MoveControlKeyboardArrow(Actor* parent):MoveControl(parent) {}
    
    virtual bool handleEvent(const SDL_Event& event) override;
};

#endif // _MOVE_CONTROL_KEYBOARD_H_