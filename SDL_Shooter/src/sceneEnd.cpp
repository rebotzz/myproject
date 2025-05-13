#include "sceneEnd.h"
#include "gameMgr.h"
#include "resourcesMgr.h"
#include "sceneBegin.h"

SceneEnd::SceneEnd()
{}

SceneEnd::~SceneEnd()
{}

void SceneEnd::enter()
{
    name.clear();
    is_editing = true;
    if(!SDL_IsTextInputActive())
        SDL_StartTextInput();
    if(!SDL_IsTextInputActive())
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_StartTextInput failed: %s", SDL_GetError());

    Mix_FadeInMusic(ResMgr::getInstance().find_music(ResID::Mus_BattleInSpaceIntro), -1, 300);
}
void SceneEnd::exit()
{
    is_editing = false;
    if(SDL_IsTextInputActive())
        SDL_StopTextInput();

    Mix_FadeOutMusic(300);
}
void SceneEnd::handleEvent(const SDL_Event& event)
{
    if(is_editing)
    {
        if(SDL_TEXTINPUT == event.type)
        {
            name += event.text.text;
        }
        else if(SDL_KEYDOWN == event.type)
        {
            if(SDL_SCANCODE_BACKSPACE == event.key.keysym.scancode)
            {
                eraseLastCharacter();
            }
            else if(SDL_SCANCODE_RETURN == event.key.keysym.scancode)
            {
                is_editing = false;
                SDL_StopTextInput();
                if(name.empty()) name = "未知用户";
                game_mgr.setRankingList(name, game_mgr.getScore());
            }
        }
    }
    else
    {
        if(SDL_KEYDOWN == event.type && SDL_SCANCODE_J == event.key.keysym.scancode)
        {
            game_mgr.switchScene(new SceneBegin);
        }
    }
}

void SceneEnd::update(double deltaTime)
{
    text_flash_time += deltaTime;
    if(text_flash_time > text_flash_cd) text_flash_time = 0;
}
void SceneEnd::render()
{
    if(is_editing)
        renderPhase1();
    else 
        renderPhase2();
}

void SceneEnd::eraseLastCharacter()
{
    if(name.empty()) return;

    // 处理中文退格
    // Ascii一个字节,第一个bit位是0
    // 中文,多个字节,第一个字节以多个1开头,后跟一个0,后续字节都以10开头
    if((name.back() & 0b10000000) == 0b10000000)
    {
        name.pop_back();
        while(!name.empty() && (name.back() & 0b11000000) != 0b11000000)
        {
            name.pop_back();
        }
    }
    name.pop_back();
}

void SceneEnd::renderPhase1()
{
    // 游戏结束,玩家输入名字
    static TTF_Font* font_64 = ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap16px, 64);
    static TTF_Font* font_24 = ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap12px, 24);
    game_mgr.renderTextCenter(font_64, "Game Over!", 0.2);
    game_mgr.renderTextCenter(font_24, "请输入玩家名字:", 0.6);
    SDL_Point point = game_mgr.renderTextCenter(font_24, name, 0.7);
    if(text_flash_time < text_flash_cd * 0.5) 
        game_mgr.renderText(font_24, "_", point.x, point.y);
}

void SceneEnd::renderPhase2()
{
    // 玩家积分排行榜
    static TTF_Font* font_64 = ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap16px, 64);
    static TTF_Font* font_24 = ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap12px, 24);
    game_mgr.renderTextCenter(font_64, "排行榜", 0.1);
    int i = 0;
    for(auto& [score, name] : game_mgr.getRankingList())
    {
        game_mgr.renderText(font_24, std::to_string(i + 1) + "." + name, 0.1, 0.2 + i * 0.06);
        game_mgr.renderText(font_24, std::to_string(score), 0.1, 0.2 + i * 0.06, false);
        i++;
    }
    if(text_flash_time < text_flash_cd * 0.5)
        game_mgr.renderTextCenter(font_24, "按 J 回到开始界面", 0.9);
}