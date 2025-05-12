#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "scene.h"
#include "object.h"
#include <vector>
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
    void updatePlayerBullet();
    void updateEnemyBullet();
    void updateProp();

    // 渲染相关
    void renderPlayer();
    void renderPlayerBullet();
    void renderEnemyBullet();
    void renderProp();

    // 其他
    void spawnPlayerBullet();
    void spawnEnemyBullet();
    void spawnProp();

private:
    Player player;
    Enemy enemy_template;
    Bullet player_bullet_template;
    Bullet enemy_bullet_template;
    Prop recover_prop_template;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> player_bullets;
    std::vector<Bullet*> enemy_bullets;
    std::vector<Prop*> props;

    // 随机数
};



#endif //_SCENE_MAIN_H_