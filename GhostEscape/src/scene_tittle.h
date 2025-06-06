#ifndef _SCENE_TITTLE_H_
#define _SCENE_TITTLE_H_

#include "core/scene.h"
#include "screen/ui_button.h"
#include "screen/hud_text.h"

class SceneTittle : public Scene
{
protected:
    UIButton* button_start_ = nullptr;
    UIButton* button_credits_ = nullptr;
    UIButton* button_quit_ = nullptr;
    HUDText* text_tittle_ = nullptr;
    HUDText* text_credits_ = nullptr;
    HUDText* text_high_score_ = nullptr;

public:
    SceneTittle();
    ~SceneTittle() = default;

    virtual void init() override;
    virtual void clean() override;
    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;
};


#endif // _SCENE_TITTLE_H_