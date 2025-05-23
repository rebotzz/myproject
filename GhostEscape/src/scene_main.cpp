#include "scene_main.h"
#include "player.h"
#include "enemy.h"
#include "core/spawner.h"
#include "raw/effect_enemy_spawn.h"
#include "ui/ui_button.h"
#include "scene_tittle.h"
#include "ui/ui_cursor.h"

SceneMain::SceneMain()
{
    world_size_ = game_.getScreenSize() * 3.0f;
    camera_position_ = glm::vec2(-100, -100);

    auto pos_player = camera_position_ + glm::vec2{game_.getScreenSize().x / 2, game_.getScreenSize().y / 2};
    player_ = Player::createAndAddPlayerChild(this, pos_player);

    // UI界面 按键
    auto button_pos = game_.getScreenSize() - glm::vec2(350.0f, 40.0f);
    auto button_scale = glm::vec2(1.3f);
    auto button_pause = UIButton::createAndAddUIButtonChild(this, button_pos, button_scale, ResID::Tex_APause1, 
        ResID::Tex_APause2, ResID::Tex_APause3);
    button_restart_ = UIButton::createAndAddUIButtonChild(this, button_pos + glm::vec2(130.0f, 0.0f), 
        button_scale, ResID::Tex_ARestart1, ResID::Tex_ARestart2, ResID::Tex_ARestart3);
    button_back_ = UIButton::createAndAddUIButtonChild(this, button_pos + glm::vec2(260.0f, 0.0f), 
        button_scale, ResID::Tex_ABack1, ResID::Tex_ABack2, ResID::Tex_ABack3);

    button_pause->setOnClickCallback([this]() 
        { 
            setPauseTime(!getIsPauseTime()); 
            if(pause_time_) game_.pauseSound();
            else game_.resumeSound();
        });
    button_restart_->setOnClickCallback([this]() { game_.safeChangeScene(new SceneMain()); });
    button_back_->setOnClickCallback([this]() { game_.safeChangeScene(new SceneTittle()); });

    ui_cursor_ = new UICursor(this, ResID::Tex_29, ResID::Tex_30, AchorMode::CENTER);
}
SceneMain::~SceneMain()
{

}
void SceneMain::init()
{
    // 恢复状态
    // 这里的工作都放在了构造
    SDL_HideCursor();
    game_.playMusic(ResID::Mus_SpookyMusic);
    player_alive_ = true;
}
void SceneMain::clean()
{
    // 清理资源
    // 这里的工作都放在了析构
    game_.pauseMusic();
    game_.updateGameData(player_->getScore());
}
bool SceneMain::handleEvent(const SDL_Event& event)
{
    return Scene::handleEvent(event);
}
void SceneMain::update(float dt)
{
    removeInvalidObject();
    Scene::update(dt);

    if(player_alive_)
    {
        // 生成敌人，相机跟随
        spawnEnemy(dt);
        setCameraPosition(player_->getPosition() - glm::vec2{game_.getScreenSize().x / 2, game_.getScreenSize().y / 2});
    }

    if(player_alive_ && player_->getIsDead())
    {
        pause_time_ = true;
        player_alive_ = false;
        button_restart_->setSize(button_restart_->getSize() * 3.0f);
        button_back_->setSize(button_back_->getSize() * 3.0f);
        button_restart_->setRenderPosition(game_.getScreenSize() / 2.0f - glm::vec2(button_back_->getSize().x * 0.7f, 0.0f));
        button_back_->setRenderPosition(game_.getScreenSize() / 2.0f + glm::vec2(button_back_->getSize().x * 0.7f, 0.0f));
    }
}
void SceneMain::render()
{
    // TODO: 超出摄像机范围不绘制，范围检测；背景改为texture或者修改绘制方法
    renderStarsBackGround();
    renderBackgroundGrid();
    Scene::render();
}

void SceneMain::renderBackgroundGrid()
{
    // 渲染坐标 = 世界坐标 - 摄像机坐标
    // 摄像机移动，相当于世界反向移动
    auto pos = - camera_position_;
    // 渲染整个世界网格；可以渲染超出屏幕的区域
    game_.renderGrid(pos, pos + world_size_, 80, 80);
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


