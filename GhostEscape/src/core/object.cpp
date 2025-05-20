#include "object.h"


Object::~Object()
{
    for(auto object : children_)
    {
        if(object) delete object;
    }
    children_.clear();
}
void Object::handleEvent(const SDL_Event& event)
{
    for(auto object : children_)
    {
        if(object->is_active_)
            object->handleEvent(event);
    }
}
void Object::update(float dt)
{
    if(!children_to_add_.empty())
    {
        for(auto object : children_to_add_)
        {
            if(object) addChild(object);
        }
        children_to_add_.clear();
    }
    for(auto object : children_)
    {
        if(object->is_active_)
            object->update(dt);
    }
}
void Object::render()
{
    for(auto object : children_)
    {
        if(object->is_active_)
            object->render();
    }
}