#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "core/scene.h"

class Player;
class UICursor;
class UIButton;
class SceneMain : public Scene
{
protected:
    Player* player_ = nullptr;
    float spawn_enemy_timer_ = 0.0f;
    float spawn_enemy_cd_ = 5.0f;
    int min_spawn_count = 3, max_spawn_count = 10;
    UICursor* ui_cursor_ = nullptr;
    UIButton* button_pause_ = nullptr;
    UIButton* button_restart_ = nullptr;
    UIButton* button_back_ = nullptr;
    bool player_alive_ = true;
    float timer_finish_countdown_ = 3.0f;

public:
    SceneMain();
    virtual ~SceneMain();

    virtual void init() override;
    virtual void clean() override;
    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // getters and setters
    Player* getPlayer() const { return player_; }

protected:
    void updateWhenGameOver(float dt);
    void renderBackgroundGrid();

    // 工具函数
    void spawnEnemy(float dt);
};


#endif // _SCENE_MAIN_H_