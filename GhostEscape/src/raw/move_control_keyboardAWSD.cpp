#include "move_control_keyboardAWSD.h"


bool MoveControlKeyboardAWSD::handleEvent(const SDL_Event& event)
{

    bool handled = true;
    switch(event.type)
    {
        case SDL_EVENT_KEY_DOWN:
            switch(event.key.scancode)
            {
                case SDL_SCANCODE_W: is_move_up_ = true; break;
                case SDL_SCANCODE_S: is_move_down_ = true; break;
                case SDL_SCANCODE_A: is_move_left_ = true; break;
                case SDL_SCANCODE_D: is_move_right_ = true; break;
                default: handled = false; break;
            }
            break;

        case SDL_EVENT_KEY_UP:
            switch(event.key.scancode)
            {
                case SDL_SCANCODE_W: is_move_up_ = false; break;
                case SDL_SCANCODE_S: is_move_down_ = false; break;
                case SDL_SCANCODE_A: is_move_left_ = false; break;
                case SDL_SCANCODE_D: is_move_right_ = false; break;
                default: handled = false; break;
            }
            break;
        default: handled = false; break;
    }

    if(handled) return handled;
    return MoveControl::handleEvent(event);
}


bool MoveControlKeyboardArrow::handleEvent(const SDL_Event& event)
{

    bool handled = true;
    switch(event.type)
    {
        case SDL_EVENT_KEY_DOWN:
            switch(event.key.scancode)
            {
                case SDL_SCANCODE_UP: is_move_up_ = true; break;
                case SDL_SCANCODE_DOWN: is_move_down_ = true; break;
                case SDL_SCANCODE_LEFT: is_move_left_ = true; break;
                case SDL_SCANCODE_RIGHT: is_move_right_ = true; break;
                default: handled = false; break;
            }
            break;

        case SDL_EVENT_KEY_UP:
            switch(event.key.scancode)
            {
                case SDL_SCANCODE_UP: is_move_up_ = false; break;
                case SDL_SCANCODE_DOWN: is_move_down_ = false; break;
                case SDL_SCANCODE_LEFT: is_move_left_ = false; break;
                case SDL_SCANCODE_RIGHT: is_move_right_ = false; break;
                default: handled = false; break;
            }
            break;
        default: handled = false; break;
    }

    if(handled) return handled;
    return MoveControl::handleEvent(event);
}
