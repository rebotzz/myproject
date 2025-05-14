#include "player.h"
#include "resourcesMgr.h"
#include "gameMgr.h"

AbstractPlayer::AbstractPlayer():game_mgr(GameMgr::getInstance())
{}

BasePlayer::BasePlayer()
{
    tex = ResMgr::getInstance().find_texture(ResID::Tex_SpaceShip);
    SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
    width /= 4;
    height /= 4;
    pos.x = game_mgr.getWindowWidth() / 2;
    pos.y = game_mgr.getWindowHeight() - 100;

    bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Laser1);
    SDL_QueryTexture(bullet_template.tex, nullptr, nullptr, &bullet_template.width, &bullet_template.height);
    bullet_template.width /= 5;
    bullet_template.height /= 5;
    bullet_template.direction = {0, -1};
    bullet_template.damage = damage;

    spawn_bullet = [](std::vector<Bullet*>& bullets, const Vector2& player_pos, int player_w, 
        int player_h, const Bullet& bullet_template)
        {
            Bullet* bullet = new Bullet(bullet_template);
            bullet->pos = player_pos;
            Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_LaserShoot4), 0);
            bullets.push_back(bullet);
        };
}

BasePlayer::~BasePlayer()
{
    for(auto& bullet : bullets)
        if(bullet) delete bullet;
    bullets.clear();
}

void BasePlayer::update(double deltaTime)
{
    if(current_hp <= 0) return;
    auto key_array = SDL_GetKeyboardState(nullptr);
    direction = {static_cast<double>(key_array[SDL_SCANCODE_D] - key_array[SDL_SCANCODE_A]),
        static_cast<double>(key_array[SDL_SCANCODE_S] - key_array[SDL_SCANCODE_W])};
    pos += direction.normalize() * speed * deltaTime;
    if(pos.x - width / 2 < 0) pos.x = width / 2;
    if(pos.x + width / 2 > game_mgr.getWindowWidth()) pos.x = game_mgr.getWindowWidth() - width / 2;
    if(pos.y - height / 2 < 0) pos.y = height / 2;
    if(pos.y + height / 2 > game_mgr.getWindowHeight()) pos.y = game_mgr.getWindowHeight() - height / 2;

    last_shoot_passed_time += deltaTime;
    if(key_array[SDL_SCANCODE_J] && last_shoot_passed_time > shoot_cd)
    {
        last_shoot_passed_time = 0;
        if(spawn_bullet) 
        {
            spawn_bullet(bullets, pos, width, height, bullet_template);
        }
    }
}

void BasePlayer::render()
{
    if(current_hp <= 0) return;
    SDL_Rect rect = {static_cast<int>(pos.x - width / 2), 
                    static_cast<int>(pos.y - height / 2), 
                    width, height};
    SDL_RenderCopy(game_mgr.getInstance().getRenderer(), tex, nullptr, &rect);
}


ShieldPlayer::ShieldPlayer(AbstractPlayer *concrete_player)
{
    player = concrete_player;
    tex_shield = ResMgr::getInstance().find_texture(ResID::Tex_Shield);
    SDL_QueryTexture(tex_shield, nullptr, nullptr, &shield_w, &shield_h);
    shield_w /= 4;
    shield_h /= 4;
}

ShieldPlayer::~ShieldPlayer()
{
    // v1.0不单独去掉包装,整体销毁,一层一层调用析构
    // delete player;      
    // v2.0由外层判断包装是否失效,单独去掉包装
}

void ShieldPlayer::update(double deltaTime)
{
    player->update(deltaTime);
    duration -= deltaTime;
    // 装饰者失效检测
    if(duration < 0) wrapper_valid = false;
    if(!player->get_wrapper_valid())
    {
        auto internal_player = player->get_interal_player();
        delete player;
        player = internal_player;
    }
}

void ShieldPlayer::render()
{
    player->render();
    // 渲染护盾
    if(duration <= 0 || player->get_current_hp() <= 0) return;
    SDL_Rect rect = { static_cast<int>(player->get_pos().x - shield_w / 2),  
        static_cast<int>(player->get_pos().y - player->get_height() / 2 - shield_h / 2 - 10),
        shield_w, shield_h};
    SDL_RenderCopy(game_mgr.getRenderer(), tex_shield, nullptr, &rect);
}

void ShieldPlayer::decrease_hp(int val)
{
    if(duration > 0 && wrapper_valid) return;
    player->decrease_hp(val);
}

TimePlayer::TimePlayer(AbstractPlayer *concrete_player)
{
    this->player = concrete_player;

    // 修改飞机图片
    SDL_Texture* tex_plane = ResMgr::getInstance().find_texture(ResID::Tex_Plane);
    SDL_QueryTexture(tex_plane, nullptr, nullptr, &width, &height);
    width /= 2;
    height /= 2;
    origin_tex = player->get_texture();
    origin_width = player->get_width();
    origin_height = player->get_height();
    player->set_texture(tex_plane);
    player->set_height(height);
    player->set_height(width);

    // 减少射击cd(效果可叠加),子弹发射逻辑
    bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Laser3);
    SDL_QueryTexture(bullet_template.tex, nullptr, nullptr, &bullet_template.width, &bullet_template.height);
    bullet_template.width /= 5;
    bullet_template.height /= 5;
    bullet_template.direction = {0, -1};
    bullet_template.damage = player->get_damage();

    origin_shoot_cd = player->get_shoot_cd();
    player->set_shoot_cd(origin_shoot_cd * 0.8);
    origin_bullet_template = player->get_bullet_template();
    player->set_bullet_template(bullet_template);
    origin_spawn_bullet = player->get_spawn_bullet_callback();
    player->set_spawn_bullet_callback(
        [](std::vector<Bullet*>& bullets, const Vector2& player_pos, int player_w, int player_h, const Bullet& bullet_template)
        {
            // this会失效,而this会传递给内层包装器,所以最好别用到[this]
            Bullet* bullet_left = new Bullet(bullet_template);
            Bullet* bullet_right = new Bullet(bullet_template);
            bullet_left->pos = player_pos - Vector2(static_cast<double>(player_w) * 0.25, 0.0);
            bullet_right->pos = player_pos + Vector2(static_cast<double>(player_w) * 0.25, 0.0);
            Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_XsLaser), 0);
            bullets.push_back(bullet_left);
            bullets.push_back(bullet_right);
        });
}

TimePlayer::~TimePlayer()
{
    // 还原
    player->set_texture(origin_tex);
    player->set_width(origin_width);
    player->set_height(origin_height);
    player->set_shoot_cd(origin_shoot_cd);
    player->set_spawn_bullet_callback(origin_spawn_bullet);
    player->set_bullet_template(origin_bullet_template);
}

void TimePlayer::update(double deltaTime)
{
    player->update(deltaTime);
    if(tmp_hp <= 1) wrapper_valid = false;
    if(!player->get_wrapper_valid())
    {
        auto internal_player = player->get_interal_player();
        delete player;
        player = internal_player;
    }
}
