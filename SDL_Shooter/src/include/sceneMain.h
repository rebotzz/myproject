#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "scene.h"
#include "object.h"
#include "player.h"
#include <vector>
#include <list>
#include <random>

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    virtual void enter() override;
    virtual void exit() override;
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(double deltaTime) override;
    virtual void render() override;

private:
    // 更新相关
    void updateEnemies(double deltaTime);
    void updateBullets(double deltaTime);
    void updateProps(double deltaTime);
    void updateExplode(double deltaTime);

    // 渲染相关
    void renderEnemies();
    void renderBullets();
    void renderProps();
    void renderExplode();
    void renderUI();

    // 其他
    void spawnEnemy();
    void spawnProp(const Vector2& pos);
    void player_hurt();

private:
    // 用于复制生成的模板
    Enemy enemy_template1;
    Enemy enemy_template2;
    Enemy enemy_template3_stone;
    Enemy enemy_template4_stone;
    Prop recover_prop_template;
    Prop shield_prop_template;
    Prop time_prop_template;
    ExplodeAnimation explode_animtion_template;
    AbstractPlayer* player;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> enemy_bullets;
    std::vector<Prop*> props;
    std::list<ExplodeAnimation*> explosion;
    int score = 0;                    // 玩家得分
    double timer_end_countdown = 3.0; // 玩家死亡场景跳转定时器

    // 随机数
    std::default_random_engine random_generator;
    std::uniform_real_distribution<double> distribution;
};



#endif //_SCENE_MAIN_H_