#ifndef _SCENE_BEGIN_H_
#define _SCENE_BEGIN_H_

#include "scene.h"
#include <string>

class SceneBegin : public Scene
{
public:
    SceneBegin() = default;
    ~SceneBegin() {};

    virtual void enter();
    virtual void exit();
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(double deltaTime) override;
    virtual void render() override;

private:
    double text_flash_cd = 1.0;
    double text_flash_time = 0;
};


#endif //_SCENE_BEGIN_H_