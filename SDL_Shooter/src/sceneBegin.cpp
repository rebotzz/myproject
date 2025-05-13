#include "sceneBegin.h"
#include "gameMgr.h"
#include "resourcesMgr.h"
#include "sceneMain.h"

void SceneBegin::enter() 
{
    Mix_FadeInMusic(ResMgr::getInstance().find_music(ResID::Mus_BattleInSpaceIntro), -1, 300);
};
void SceneBegin::exit() 
{
    Mix_FadeOutMusic(300);
};
void SceneBegin::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_J)
    {
        game_mgr.switchScene(new SceneMain);
        SDL_ResetKeyboard();
    }
}
void SceneBegin::update(double deltaTime)
{
    text_flash_time += deltaTime;
    if(text_flash_time > text_flash_cd) text_flash_time = 0;
}
void SceneBegin::render()
{
    game_mgr.renderTextCenter(ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap16px, 64), "SDL与太空战机", 0.2);
    if(text_flash_time < text_flash_cd * 0.5)
    {
        game_mgr.renderTextCenter(ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap12px, 24), "按 J 开始游戏!", 0.8);
    }
}