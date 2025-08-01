#include "scene_main.h"
#include "player.h"
#include "enemy.h"
#include "raw/spawner.h"
#include "screen/ui_button.h"
#include "scene_tittle.h"
#include "screen/ui_cursor.h"
#include "affiliate/collide_manager.h"
#include "raw/spawner.h"
#include "screen/ui_player_status.h"

SceneMain::SceneMain()
{
    // 初始化世界大小和相机位置
    world_size_ = game_.getScreenSize() * 3.0f;
    camera_position_ = world_size_ / 2.0f - game_.getScreenSize() / 2.0f;

    // UI界面 按键 光标
    auto button_pos = game_.getScreenSize() * glm::vec2(0.7f, 0.93f);
    auto button_delta_pos = glm::vec2(game_.getScreenSize().x * 0.1f, 0.0f);
    auto button_scale = glm::vec2(1.4f);
    button_pause_ = UIButton::createAndAddUIButtonChild(this, button_pos, button_scale, ResID::Tex_APause1, 
        ResID::Tex_APause2, ResID::Tex_APause3);
    button_restart_ = UIButton::createAndAddUIButtonChild(this, button_pos + button_delta_pos, 
        button_scale, ResID::Tex_ARestart1, ResID::Tex_ARestart2, ResID::Tex_ARestart3);
    button_back_ = UIButton::createAndAddUIButtonChild(this, button_pos + button_delta_pos * 2.0f, 
        button_scale, ResID::Tex_ABack1, ResID::Tex_ABack2, ResID::Tex_ABack3);
    ui_cursor_ = new UICursor(this, ResID::Tex_29, ResID::Tex_30, AchorMode::CENTER);

    button_pause_->setOnClickCallback([this]() 
        { 
            setPauseTime(!getIsPauseTime()); 
            if(pause_time_) game_.pauseSound();
            else game_.resumeSound();
        });
    button_restart_->setOnClickCallback([this]() { game_.safeChangeScene(new SceneMain()); });
    button_back_->setOnClickCallback([this]() { game_.safeChangeScene(new SceneTittle()); });

}
SceneMain::~SceneMain()
{

}
void SceneMain::init()
{
    // 初始化场景，恢复状态
    // 创建场景工作太少，所以，这里的大部分工作都放在了构造
    SDL_HideCursor();
    game_.playMusic(ResID::Mus_SpookyMusic);
    player_alive_ = true;

    // 初始化碰撞管理器
    getCollideMgr()->reinit();

    // 引入碰撞管理器后，玩家等持有碰撞盒的对象需要在碰撞管理器之后初始化
    // 初始化玩家
    player_ = new Player(this, world_size_ * 0.5f);
    // 初始化敌人生成器
    new EnemySpawner(this, player_, glm::ivec2{3, 6}, glm::vec2(5, 7));

    // UI界面 挂载到场景
    new UIPlayerStatus(this, player_->getWeapon1(), player_->getWeapon2());
}
void SceneMain::clean()
{
    // 清理资源
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
    checkSlowdown(dt);
    Scene::update(dt);
    updateWhenGameOver(dt);
}

void SceneMain::render()
{
    // TODO: 超出摄像机范围不绘制，范围检测；背景改为texture或者修改绘制方法
    renderStarsBackGround();
    renderBackgroundGrid();
    Scene::render();
}

void SceneMain::updateWhenGameOver(float dt)
{
    if(!player_alive_)
    {
        timer_finish_countdown_ -= dt;
        if(timer_finish_countdown_ < 0.0f && button_pause_->getIsActive())
        {
            button_pause_->setActive(false);
            setPauseTime(true);
            button_restart_->setSize(button_restart_->getSize() * 2.0f);
            button_back_->setSize(button_back_->getSize() * 2.0f);
            button_restart_->setRenderPosition(game_.getScreenSize() / 2.0f - glm::vec2(button_back_->getSize().x * 0.7f, 0.0f));
            button_back_->setRenderPosition(game_.getScreenSize() / 2.0f + glm::vec2(button_back_->getSize().x * 0.7f, 0.0f));
        }
    }

    if(player_alive_ && player_->getIsDead())
    {
        player_alive_ = false;
    }
}

void SceneMain::renderBackgroundGrid()
{
    // 渲染坐标 = 世界坐标 - 摄像机坐标
    // 摄像机移动，相当于世界反向移动
    auto pos = - camera_position_;
    // 渲染整个世界网格；可以渲染超出屏幕的区域
    game_.renderGrid(pos, pos + world_size_, 80, 80, SDL_FColor{0.5f, 0.5f, 0.5f, 1.0f});
    game_.renderBoundary(pos, pos + world_size_, 5);
}

void SceneMain::checkSlowdown(float& dt)
{
    // 游戏减速：鼠标中键按下
    glm::vec2 mouse_pos;
    if(game_.getMouseState(mouse_pos) & SDL_BUTTON_MMASK)
    {
        dt *= 0.4f;
    }
}
