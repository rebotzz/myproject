#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "core/scene.h"

class Player;
class SceneMain : public Scene
{
protected:
    glm::vec2 world_size_ = glm::vec2(0);  // 世界大小
    Player* player_ = nullptr;
    float spawn_enemy_timer_ = 0.0f;
    float spawn_enemy_cd_ = 8.0f;

public:
    SceneMain();
    virtual ~SceneMain();

    virtual void init();
    virtual void clean();
    virtual void handleEvent(const SDL_Event& event);
    virtual void update(float dt);
    virtual void render();

    // getters and setters
    Player* getPlayer() const { return player_; }
    std::vector<Object*>& getWorldObjects() { return world_objects_; }

protected:
    void renderBackground();

    // 工具函数
    void spawnEnemy(float dt);
};


#endif // _SCENE_MAIN_H_