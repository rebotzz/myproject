#include "object.h"

Object::~Object()
{
    for(auto& child : children_)
    {
        if(child) delete child;
        child = nullptr;
    }
    children_.clear();
}

void Object::handleEvent(const SDL_Event &event)
{
    for(auto child : children_)
    {
        if(child) child->handleEvent(event);
    }
}

void Object::update(float delta_time)
{
    for(auto child : children_)
    {
        if(child) child->update(delta_time);
    }
}

void Object::render()
{
    for(auto child : children_)
    {
        if(child) child->render();
    }
}
