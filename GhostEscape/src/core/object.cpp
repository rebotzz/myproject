#include "object.h"
#include <algorithm>


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
    // 移除失效子节点
    removeInvalidObject();
    // 加入待添加子节点，更新
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

void Object::removeInvalidObject()
{
    children_.erase(std::remove_if(children_.begin(), children_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), children_.end());
}
