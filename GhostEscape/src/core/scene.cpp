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

bool Scene::handleEvent(const SDL_Event& event)
{
    if(Object::handleEvent(event)) return true;

    // 优先处理屏幕事件，然后世界事件
    for(auto obj : screen_objects_)
    {
        if(obj->getIsActive()) 
        {
            if(obj->handleEvent(event)) return true;
        }
    }

    if(!pause_time_)
    {
        for(auto obj : world_objects_)
        {
            if(obj->getIsActive()) 
            {
                if(obj->handleEvent(event)) return true;
            }
        }
    }

    return false;
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

    // 时间暂停时不更新世界对象
    if(!pause_time_)
    {
        for(auto obj : world_objects_)
        {
            if(obj->getIsActive()) obj->update(dt);
        }
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

void Scene::renderStarsBackGround()
{
    // 初始化星星世界坐标
    std::vector<std::vector<glm::vec2>*> stars_ptr_arr = {&bg_stars, &mid_stars, &fg_stars};
    if(!init_stars)
    {
        init_stars = true;
        bg_stars.reserve(stars_count);
        mid_stars.reserve(stars_count);
        fg_stars.reserve(stars_count);
        for(auto& stars_ptr : stars_ptr_arr)
        {
            auto& stars = *stars_ptr;
            for(int i = 0; i < stars_count; ++i)
            {
                stars.push_back(game_.getRandomVec2(glm::vec2(0), world_size_));
            }
        }
    }

    // 渲染星星
    static std::vector<float> scale = {0.3f, 0.6f, 1.0f};
    static std::vector<SDL_FColor> color = {SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.5f, 0.0f, 1.0f}};
    int idx = 0;
    for(auto& stars_ptr : stars_ptr_arr)
    {
        auto& stars = *stars_ptr;
        for(int i = 0; i < stars_count; ++i)
        {
            // 渲染坐标 = 世界坐标 - 相机位置 * 系数
            auto pos = stars[i] - camera_position_ * scale[idx];
            game_.renderFillRect(pos, glm::vec2(1), color[idx]);
        }
        idx++;
    }

}
