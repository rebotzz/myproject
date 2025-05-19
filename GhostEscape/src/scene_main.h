#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "core/scene.h"

class Player;
class SceneMain : public Scene
{
protected:
    glm::vec2 world_size_ = glm::vec2(0);  // 世界大小
    Player* player_ = nullptr;

public:
    SceneMain();
    virtual ~SceneMain();

    virtual void init();
    virtual void clean();
    virtual void handleEvent(const SDL_Event& event);
    virtual void update(float dt);
    virtual void render();

protected:
    void renderBackground();
};


#endif // _SCENE_MAIN_H_