#include "scene_main.h"
#include "player.h"
#include "enemy.h"
#include "core/spawner.h"
#include "raw/effect_enemy_spawn.h"


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
    spawnEnemy(dt);
    auto target = player_->getPosition() - glm::vec2{game_.getScreenSize().x / 2, game_.getScreenSize().y / 2};
    cameraFollow(target);

    removeInvalidObject();
    Scene::update(dt);
}
void SceneMain::render()
{
    // TODO: 超出摄像机范围不绘制，范围检测；背景改为texture或者修改绘制方法
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
    game_.renderBoundary(pos, pos + world_size_, 5);
}

void SceneMain::spawnEnemy(float dt)
{
    spawn_enemy_timer_ += dt;
    if(spawn_enemy_timer_ >= spawn_enemy_cd_)
    {
        spawn_enemy_timer_ -= spawn_enemy_cd_;
        // 世界坐标
        auto pos_start =  glm::clamp(camera_position_, glm::vec2(0), world_size_);
        auto pos_end = glm::clamp(camera_position_ + game_.getScreenSize(), glm::vec2(0), world_size_);
        auto spawner = Spawner<EffectEnemySpawn>(game_.getRandom(min_spawn_count, max_spawn_count), pos_start, pos_end);
        spawner.spawnAndAddChild(this);
    }
}


