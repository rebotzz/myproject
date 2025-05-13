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
    
private:
    std::string name;
    bool is_editing = true;
};



#endif //_SCENE_END_H_