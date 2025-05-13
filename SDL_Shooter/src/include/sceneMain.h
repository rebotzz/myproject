#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "scene.h"
#include "object.h"
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
    void updatePlayer(double deltaTime);
    void updateEnemies(double deltaTime);
    void updateBullets(double deltaTime);
    void updateProps(double deltaTime);

    // 渲染相关
    void renderPlayer();
    void renderEnemies();
    void renderBullets();
    void renderProps();
    void renderExplode();
    void renderUI();

    // 其他
    void spawnEnemy();
    void spawnPlayerBullet();
    void spawnProp(const Vector2& pos);
    void player_hurt();

private:
    // 用于复制生成的模板
    Player player_template;
    Enemy enemy_template;
    Bullet player_bullet_template;
    Bullet enemy_bullet_template;
    Prop recover_prop_template;
    ExplodeAnimation explode_animtion_template;
    Player player;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> player_bullets;
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