#include "scene_tittle.h"
#include "ui/ui_button.h"
#include "ui/ui_cursor.h"
#include "scene_main.h"

SceneTittle::SceneTittle()
{
    stars_count = 500;
    world_size_ = game_.getScreenSize();
    auto tittle_pos = glm::vec2(game_.getScreenSize().x / 2, game_.getScreenSize().y * 0.3);
    text_tittle_ = new HUDText(this, tittle_pos, "幽灵逃生", 128, ResID::Tex_Textfield01);

    std::string high_score = "历史最高分:" + std::to_string(game_.getGameData());
    auto score_pos = glm::vec2(game_.getScreenSize().x / 2, game_.getScreenSize().y * 0.6);
    text_high_score_ = new HUDText(this, score_pos, high_score, 32, ResID::Tex_Textfield01);

    auto button_pos = glm::vec2(game_.getScreenSize().x / 2, game_.getScreenSize().y * 0.8);
    auto button_offset = glm::vec2(200.f, 0);
    button_start_ = new UIButton(this, button_pos - button_offset, glm::vec2(2.0f), ResID::Tex_AStart1, 
        ResID::Tex_AStart2, ResID::Tex_AStart3);
    button_credits_ = new UIButton(this, button_pos, glm::vec2(2.0f), ResID::Tex_ACredits1, 
        ResID::Tex_ACredits2, ResID::Tex_ACredits3);
    button_quit_ = new UIButton(this, button_pos + button_offset, glm::vec2(2.0f), ResID::Tex_AQuit1, 
        ResID::Tex_AQuit2, ResID::Tex_AQuit3);

    text_credits_ = new HUDText(this, game_.getScreenSize() * 0.5f, game_.getAssetStore().getText(ResID::Text_Credits), 
        16, ResID::Tex_Textfield01);
    text_credits_->setActive(false);

    button_start_->setOnClickCallback([this](){ game_.safeChangeScene(new SceneMain()); });
    button_credits_->setOnClickCallback([this](){ show_credits_ = true; text_credits_->setActive(true); });
    button_quit_->setOnClickCallback([this](){ game_.quit(); });

    new UICursor(this, ResID::Tex_PointerCShaded, ResID::Tex_PointerCShaded);
}

void SceneTittle::init()
{
    SDL_HideCursor();
    game_.playMusic(ResID::Mus_OhMyGhost);
    text_high_score_->setText("历史最高分:" + std::to_string(game_.getGameData()));
}

void SceneTittle::clean()
{
    game_.pauseMusic();
}


bool SceneTittle::handleEvent(const SDL_Event& event)
{
    if(show_credits_ && event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        show_credits_ = false;
        text_credits_->setActive(false);
        return true;
    }

    return Scene::handleEvent(event);
}
void SceneTittle::update(float dt)
{
    Scene::update(dt);
}
void SceneTittle::render()
{
    renderStarsBackGround();
    Scene::render();
}

void SceneTittle::renderBackground()
{
    // todo
}
