#include "scene_main.h"
#include "player.h"


SceneMain::SceneMain()
{
    world_size_ = game_.getScreenSize() * 3.0f;
    camera_position_ = glm::vec2(-100, -100);

    auto pos_player = camera_position_ + glm::vec2{game_.getScreenSize().x / 2, game_.getScreenSize().y / 2};
    player_ = Player::createAndAddPlayerChild(this, pos_player);
}
SceneMain::~SceneMain()
{

}
void SceneMain::init()
{

}
void SceneMain::clean()
{

}
void SceneMain::handleEvent(const SDL_Event& event)
{
    Scene::handleEvent(event);
}
void SceneMain::update(float dt)
{
    Scene::update(dt);
    auto target = player_->getPosition() - glm::vec2{game_.getScreenSize().x / 2, game_.getScreenSize().y / 2};
    camera_position_ = target;
}
void SceneMain::render()
{
    renderBackground();
    Scene::render();

}

void SceneMain::renderBackground()
{
    // 渲染坐标 = 世界坐标 - 摄像机坐标
    // 摄像机移动，相当于世界反向移动
    auto pos = - camera_position_;
    // 渲染整个世界网格；可以渲染超出屏幕的区域
    game_.drawGrid(pos, pos + world_size_, 80, 80);
    game_.drawBoundary(pos, pos + world_size_, 5);
}
