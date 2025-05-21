#include "scene.h"
#include <algorithm>

Scene::~Scene()
{
    for(auto obj : screen_objects_)
    {
        if(obj) delete obj;
    }
    for(auto obj : world_objects_)
    {
        if(obj) delete obj;
    }
    screen_objects_.clear();
    world_objects_.clear();
}

void Scene::handleEvent(const SDL_Event& event)
{
    Object::handleEvent(event);
    for(auto obj : screen_objects_)
    {
        if(obj->getIsActive()) obj->handleEvent(event);
    }

    for(auto obj : world_objects_)
    {
        if(obj->getIsActive()) obj->handleEvent(event);
    }
}
void Scene::update(float dt)
{
    removeInvalidObject();
    if(!children_to_add_.empty())
    {
        for(auto obj : children_to_add_)
        {
            if(obj) addChild(obj);
        }
        children_to_add_.clear();
    }
    Object::update(dt);

    for(auto obj : screen_objects_)
    {
        if(obj->getIsActive()) obj->update(dt);
    }

    for(auto obj : world_objects_)
    {
        if(obj->getIsActive()) obj->update(dt);
    }

    // // 限制摄像机位置, 似乎限制玩家位置就可以了, 有bug，坐标位置映射不对
    // camera_position_ = glm::clamp(camera_position_, glm::vec2(-100), world_size_ + glm::vec2(100));
}
void Scene::render()
{
    Object::render();

    for(auto obj : world_objects_)
    {
        if(obj->getIsActive()) obj->render();
    }
    for(auto obj : screen_objects_)
    {
        if(obj->getIsActive()) obj->render();
    }
}

void Scene::addChild(Object* object)
{
    switch(object->getObjectType())
    {
        case ObjectType::None: children_.push_back(object); break;
        case ObjectType::Screen: screen_objects_.push_back(object); break;
        case ObjectType::World: 
        case ObjectType::Enemy: 
        case ObjectType::Player: 
        world_objects_.push_back(object); 
        break;
    }
}

void Scene::safeAddChild(Object* object)
{
    Object::safeAddChild(object);
}

void Scene::removeInvalidObject()
{
    world_objects_.erase(std::remove_if(world_objects_.begin(), world_objects_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), world_objects_.end());

    screen_objects_.erase(std::remove_if(screen_objects_.begin(), screen_objects_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), screen_objects_.end());
}