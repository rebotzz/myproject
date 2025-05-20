#include "scene.h"

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
    Object::update(dt);

    for(auto obj : screen_objects_)
    {
        if(obj->getIsActive()) obj->update(dt);
    }

    for(auto obj : world_objects_)
    {
        if(obj->getIsActive()) obj->update(dt);
    }
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