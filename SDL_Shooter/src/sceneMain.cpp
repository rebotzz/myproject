#include "sceneMain.h"
#include "resourcesMgr.h"
#include "vector2.h"
#include "gameMgr.h"

SceneMain::SceneMain(/* args */)
{
    player.tex = ResMgr::getInstance().find_texture(ResID::Tex_SpaceShip);
    SDL_QueryTexture(player.tex, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.pos.x = game_mgr.getWindowWidth() / 2;
    player.pos.y = game_mgr.getWindowHeight() - 100;

    enemy_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Insect2);
    SDL_QueryTexture(enemy_template.tex, nullptr, nullptr, &enemy_template.width, &enemy_template.height);
    enemy_template.width /= 4;
    enemy_template.height /= 4;

    player_bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Laser1);
    SDL_QueryTexture(player_bullet_template.tex, nullptr, nullptr, &player_bullet_template.width, &player_bullet_template.height);
    player_bullet_template.width /= 4;
    player_bullet_template.height /= 4;

    enemy_bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Bullet1);
    SDL_QueryTexture(enemy_bullet_template.tex, nullptr, nullptr, &enemy_bullet_template.width, &enemy_bullet_template.height);
    enemy_bullet_template.width /= 4;
    enemy_bullet_template.height /= 4;

    recover_prop_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_BonusLife);
    SDL_QueryTexture(recover_prop_template.tex, nullptr, nullptr, &recover_prop_template.width, &recover_prop_template.height);
    recover_prop_template.width /= 4;
    recover_prop_template.height /= 4;
}

SceneMain::~SceneMain()
{

}


void SceneMain::enter()
{

}

void SceneMain::exit()
{
    
}

void SceneMain::handleEvent(const SDL_Event& event)
{

}

void SceneMain::update(double deltaTime)
{
    updatePlayer(deltaTime);
}

void SceneMain::render()
{
    renderPlayer();
}

void SceneMain::updatePlayer(double deltaTime)
{
    auto key_array = SDL_GetKeyboardState(nullptr);
    Vector2 velocity = {static_cast<double>(key_array[SDL_SCANCODE_D] - key_array[SDL_SCANCODE_A]),
        static_cast<double>(key_array[SDL_SCANCODE_S] - key_array[SDL_SCANCODE_W])};
    player.pos += velocity.normalize() * player.speed * deltaTime;
    if(player.pos.x < 0) player.pos.x = 0;
    if(player.pos.x + player.width > game_mgr.getWindowWidth()) player.pos.x = game_mgr.getWindowWidth() - player.width;
    if(player.pos.y < 0) player.pos.y = 0;
    if(player.pos.y + player.height > game_mgr.getWindowHeight()) player.pos.y = game_mgr.getWindowHeight() - player.height;
}

void SceneMain::renderPlayer()
{
    SDL_Rect rect = {static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), 
        static_cast<int>(player.width), static_cast<int>(player.height)};
    SDL_RenderCopy(game_mgr.getInstance().getRenderer(), player.tex, nullptr, &rect);
}
