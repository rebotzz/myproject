#ifndef _SCENE_END_H_
#define _SCENE_END_H_

#include "scene.h"
#include <string>

class SceneEnd : public Scene
{
public:
    SceneEnd();
    ~SceneEnd();

    virtual void enter() override;
    virtual void exit() override;
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(double deltaTime) override;
    virtual void render() override;

private:
    void eraseLastCharacter();
    void renderPhase1();
    void renderPhase2();
    
private:
    std::string name;
    bool is_editing = true;
    double text_flash_cd = 1.0;
    double text_flash_time = 0;
};


#endif //_SCENE_END_H_