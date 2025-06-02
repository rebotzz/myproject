#include "object.h"
#include <algorithm>


Object::Object(Object* parent, ObjectType type) 
    :type_(type)
{ 
    if(parent) 
    { 
        setParent(parent); 
        parent->safeAddChild(this); 
    } 
}

Object::~Object()
{
    for(auto object : children_)
    {
        if(object) delete object;
    }
    children_.clear();
    
    for(auto object : children_to_add_)
    {
        if(object) delete object;
    }
    children_to_add_.clear();
}
bool Object::handleEvent(const SDL_Event& event)
{
    for(auto object : children_)
    {
        if(object->is_active_)
        {
            if(object->handleEvent(event)) return true;
        }
    }
    return false;
}
void Object::update(float dt)
{
    // 移除失效子节点
    deleteInvalidObject();
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

void Object::removeChild(Object* object)
{ 
    children_.erase(std::remove(children_.begin(), children_.end(), object), children_.end());
    children_to_add_.erase(std::remove(children_to_add_.begin(), children_to_add_.end(), object), 
    children_to_add_.end());
}

void Object::deleteInvalidObject()
{
    children_.erase(std::remove_if(children_.begin(), children_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), children_.end());
}
