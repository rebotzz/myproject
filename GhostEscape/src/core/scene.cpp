#include "scene.h"
#include <algorithm>

Scene::~Scene()
{
    for(auto obj : screen_children_)
    {
        if(obj) delete obj;
    }
    for(auto obj : world_children_)
    {
        if(obj) delete obj;
    }
    screen_children_.clear();
    world_children_.clear();
}

bool Scene::handleEvent(const SDL_Event& event)
{
    if(Object::handleEvent(event)) return true;

    // 优先处理屏幕事件，然后世界事件
    for(auto obj : screen_children_)
    {
        if(obj->getIsActive()) 
        {
            if(obj->handleEvent(event)) return true;
        }
    }

    if(!pause_time_)
    {
        for(auto obj : world_children_)
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

    for(auto obj : screen_children_)
    {
        if(obj->getIsActive()) obj->update(dt);
    }

    // 时间暂停时不更新世界对象
    if(!pause_time_)
    {
        for(auto obj : world_children_)
        {
            if(obj->getIsActive()) obj->update(dt);
        }
    }

    updateCamera(dt);
}

void Scene::render()
{
    Object::render();

    for(auto obj : world_children_)
    {
        if(obj->getIsActive()) obj->render();
    }
    for(auto obj : screen_children_)
    {
        if(obj->getIsActive()) obj->render();
    }
}

void Scene::addChild(Object* object)
{
    switch(object->getObjectType())
    {
        case ObjectType::None: children_.push_back(object); break;
        case ObjectType::Screen: screen_children_.push_back(object); break;
        case ObjectType::World: 
        case ObjectType::Enemy: 
        case ObjectType::Player: 
        default:
        world_children_.push_back(object); 
        break;
    }
}

void Scene::removeChild(Object* object) 
{
    switch(object->getObjectType())
    {
        case ObjectType::None: 
        Object::removeChild(object); 
        break;
        case ObjectType::Screen: 
        screen_children_.erase(std::remove(screen_children_.begin(),screen_children_.end(), object), screen_children_.end());
        break;
        case ObjectType::World: 
        case ObjectType::Enemy: 
        case ObjectType::Player: 
        world_children_.erase(std::remove(world_children_.begin(),world_children_.end(), object), world_children_.end());
        break;
    }
}

void Scene::cameraFollow(float dt, const glm::vec2 &target)
{
    // 使用线性插值实现平滑移动
    float smoothness = 5.0f;    // 平滑系数，值越大移动越快
    camera_position_ += (target - camera_position_) * smoothness * dt;
}

bool Scene::checkBeyoundScreen(const glm::vec2 &world_position, const glm::vec2 &size, const glm::vec2 &margin)
{
    auto screen_size = game_.getScreenSize();
    if(world_position.x - size.x / 2 < camera_position_.x - margin.x || 
       world_position.y - size.y / 2 < camera_position_.y - margin.y ||
       world_position.x + size.x / 2 > camera_position_.x + screen_size.x + margin.x || 
       world_position.y + size.y / 2 > camera_position_.y + screen_size.y + margin.y)
    {
        return true;
    }
    return false;
}

void Scene::setCameraZoom(float camera_zoom)
{
    camera_zoom_ = camera_zoom;
    game_.setSDL_RenderScale(camera_zoom_); // todo: 修改渲染坐标
}

void Scene::setCameraPosition(const glm::vec2 &position)
{
    camera_position_ = position;
    // 对相机位置进行限制,不超出世界边界太多
    camera_position_ = glm::clamp(camera_position_, glm::vec2(-50), world_size_ - game_.getScreenSize() + glm::vec2(50));
}

void Scene::updateCamera(float)
{
    // 限制摄像机位置
    camera_position_ = glm::clamp(camera_position_, glm::vec2(-50), world_size_ - game_.getScreenSize() + glm::vec2(50));
}

void Scene::removeInvalidObject()
{
    world_children_.erase(std::remove_if(world_children_.begin(), world_children_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), world_children_.end());

    screen_children_.erase(std::remove_if(screen_children_.begin(), screen_children_.end(), [](Object* obj)
    {
        bool deletable = false;
        if(obj->getCanRemove()) deletable = true;
        if(deletable) delete obj;
        return deletable;
    }), screen_children_.end());
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
    static std::vector<float> distance_scale = {0.3f, 0.6f, 1.0f};
    static std::vector<SDL_FColor> color = { SDL_FColor{1.0f, 0.5f, 0.0f, 1.0f}, 
        SDL_FColor{0.0f, 1.0f, 1.0f, 1.0f}, SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f}};
    int idx = 0;
    for(auto& stars_ptr : stars_ptr_arr)
    {
        auto& stars = *stars_ptr;
        for(int i = 0; i < stars_count; ++i)
        {
            // 渲染坐标 = 世界坐标 - 相机位置 * 系数
            auto pos = stars[i] - camera_position_ * distance_scale[idx];
            game_.renderFillRect(pos, glm::vec2(static_cast<float>(1 + idx / 2)), color[idx]);
        }
        idx++;
    }

}
