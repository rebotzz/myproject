#include "scene.h"

Scene::~Scene()
{

}

void Scene::handleEvent(const SDL_Event& event)
{
    Object::handleEvent(event);
}
void Scene::update(float dt)
{
    Object::update(dt);
}
void Scene::render()
{
    Object::render();
}

void Scene::cameraFollow(const glm::vec2 &target, float speed, float dt)
{
    auto distance = glm::length(target - camera_position_);
    if(distance < 0.1) return;

    auto direction = glm::normalize(target - camera_position_);
    camera_position_ += direction * speed * dt;
}
