#include "sceneEnd.h"
#include "gameMgr.h"
#include "resourcesMgr.h"
#include "sceneBegin.h"

SceneEnd::SceneEnd()
{
}

SceneEnd::~SceneEnd()
{
}

void SceneEnd::enter()
{
    name.clear();
    is_editing = true;
    if(!SDL_IsTextInputActive())
        SDL_StartTextInput();
    if(!SDL_IsTextInputActive())
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_StartTextInput failed: %s", SDL_GetError());
}
void SceneEnd::exit()
{
    is_editing = false;
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

}
void SceneEnd::render()
{
    game_mgr.renderTextCenter(ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap16px, 64), "Game Over!", 0.2);
    game_mgr.renderTextCenter(ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap12px, 24), "请输入玩家名字:", 0.6);
    

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
