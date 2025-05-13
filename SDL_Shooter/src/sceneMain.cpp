#include "sceneMain.h"
#include "resourcesMgr.h"
#include "vector2.h"
#include "gameMgr.h"
#include "sceneEnd.h"
#include <algorithm>

SceneMain::SceneMain()
{
    distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    player_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_SpaceShip);
    SDL_QueryTexture(player_template.tex, nullptr, nullptr, &player_template.width, &player_template.height);
    player_template.width /= 4;
    player_template.height /= 4;
    player_template.pos.x = game_mgr.getWindowWidth() / 2;
    player_template.pos.y = game_mgr.getWindowHeight() - 100;

    enemy_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Insect2);
    SDL_QueryTexture(enemy_template.tex, nullptr, nullptr, &enemy_template.width, &enemy_template.height);
    enemy_template.width /= 4;
    enemy_template.height /= 4;
    enemy_template.direction = {0, 1};

    player_bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Laser1);
    SDL_QueryTexture(player_bullet_template.tex, nullptr, nullptr, &player_bullet_template.width, &player_bullet_template.height);
    player_bullet_template.width /= 5;
    player_bullet_template.height /= 5;
    player_bullet_template.direction = {0, -1};
    player_bullet_template.damage = player_template.damage;

    enemy_bullet_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Bullet1);
    SDL_QueryTexture(enemy_bullet_template.tex, nullptr, nullptr, &enemy_bullet_template.width, &enemy_bullet_template.height);
    enemy_bullet_template.width /= 4;
    enemy_bullet_template.height /= 4;
    enemy_bullet_template.damage = enemy_template.damage;
    enemy_bullet_template.speed *= 0.7;

    recover_prop_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_BonusLife);
    SDL_QueryTexture(recover_prop_template.tex, nullptr, nullptr, &recover_prop_template.width, &recover_prop_template.height);
    recover_prop_template.width /= 4;
    recover_prop_template.height /= 4;
    recover_prop_template.type = PropType::Recover;

    explode_animtion_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Explosion);
    SDL_QueryTexture(explode_animtion_template.tex, nullptr, nullptr, &explode_animtion_template.width, &explode_animtion_template.height);
    explode_animtion_template.total_frame = explode_animtion_template.width / explode_animtion_template.height;
    explode_animtion_template.width /= explode_animtion_template.total_frame;
}

SceneMain::~SceneMain()
{

}


void SceneMain::enter()
{
    player = player_template;
    score = 0;
    player_dead_pass_time = 0;
}

void SceneMain::exit()
{
    for(auto& enemy : enemies)
        if(enemy) delete enemy;
    for(auto& bullet : player_bullets)
        if(bullet) delete bullet;
    for(auto& bullet : enemy_bullets)
        if(bullet) delete bullet;
    enemies.clear();
    player_bullets.clear();
    enemy_bullets.clear();
}

void SceneMain::handleEvent(const SDL_Event&)
{}

void SceneMain::update(double deltaTime)
{
    if(player.current_hp <= 0)
    {
        player_dead_pass_time += deltaTime;
        if(player_dead_pass_time > timer_end) 
        {
            game_mgr.switchScene(new SceneEnd);
        }
    }
    spawnEnemy();
    updatePlayer(deltaTime);
    updateEnemies(deltaTime);
    updateBullets(deltaTime);
    updateProps(deltaTime);
}

void SceneMain::render()
{
    renderPlayer();
    renderEnemies();
    renderBullets();
    renderProps();
    renderExplode();
    renderUI();
}

void SceneMain::updatePlayer(double deltaTime)
{
    auto key_array = SDL_GetKeyboardState(nullptr);
    player.direction = {static_cast<double>(key_array[SDL_SCANCODE_D] - key_array[SDL_SCANCODE_A]),
        static_cast<double>(key_array[SDL_SCANCODE_S] - key_array[SDL_SCANCODE_W])};
    player.pos += player.direction.normalize() * player.speed * deltaTime;
    if(player.pos.x < 0) player.pos.x = 0;
    if(player.pos.x + player.width > game_mgr.getWindowWidth()) player.pos.x = game_mgr.getWindowWidth() - player.width;
    if(player.pos.y < 0) player.pos.y = 0;
    if(player.pos.y + player.height > game_mgr.getWindowHeight()) player.pos.y = game_mgr.getWindowHeight() - player.height;

    player.last_shoot_passed_time += deltaTime;
    if(key_array[SDL_SCANCODE_J] && player.last_shoot_passed_time > player.shoot_cd)
    {
        player.last_shoot_passed_time = 0;
        spawnPlayerBullet();
    }
}

void SceneMain::updateEnemies(double deltaTime)
{
    for(auto& enemy : enemies)
    {
        enemy->pos += enemy->direction * enemy->speed * deltaTime;
        // 超出屏幕
        if(enemy->pos.x + enemy->width < 0 
            || enemy->pos.y + enemy->height < -100
            || enemy->pos.x > game_mgr.getWindowWidth()
            || enemy->pos.y > game_mgr.getWindowHeight())
            {
                enemy->valid = false;
                continue;
            }

        // 发射子弹
        enemy->last_shoot_passed_time += deltaTime;
        if(enemy->current_hp > 0 && player.current_hp > 0 && enemy->last_shoot_passed_time > enemy->shoot_cd)
        {
            enemy->last_shoot_passed_time = 0;
            Bullet* bullet = new Bullet(enemy_bullet_template);
            bullet->direction = (player.pos - enemy->pos).normalize();
            bullet->pos = enemy->pos;
            enemy_bullets.push_back(bullet);
        }

        // 与玩家相撞
        SDL_Rect rect_player = {static_cast<int>(player.pos.x - player.width / 2),
            static_cast<int>(player.pos.y - player.height / 2),
            player.width, player.height};
        SDL_Rect rect_enemy = {static_cast<int>(enemy->pos.x - enemy->width / 2),
            static_cast<int>(enemy->pos.y - enemy->height / 2),
            enemy->width, enemy->height};
        if(SDL_HasIntersection(&rect_player, &rect_enemy))
        {
            player.current_hp -= enemy->damage;
            if(player.current_hp < 0) player.current_hp = 0;
            enemy->current_hp = 0;
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [this](Enemy* enemy)
        {
            bool deletable = false;
            if(enemy->current_hp <= 0 || !enemy->valid) 
            {
                if(enemy->current_hp <= 0) 
                {
                    score += 10;
                    spawnProp(enemy->pos);
                    auto anim_explosion = new ExplodeAnimation(explode_animtion_template);
                    anim_explosion->pos = enemy->pos;
                    explosion.push_back(anim_explosion);
                }
                deletable = true;
                delete enemy;
            }
            return deletable;
        }), enemies.end());
}



void SceneMain::updateBullets(double deltaTime)
{
    // 更新玩家子弹
    for(auto& bullet : player_bullets)
    {
        bullet->pos += bullet->direction * bullet->speed * deltaTime;
        // 超出屏幕
        if(bullet->pos.x + bullet->width < 0 
            || bullet->pos.y + bullet->height < 0
            || bullet->pos.x > game_mgr.getWindowWidth()
            || bullet->pos.y > game_mgr.getWindowHeight())
            {
                bullet->valid = false;
                continue;
            }
        // 与敌人碰撞
        SDL_Rect rect_bullet = {static_cast<int>(bullet->pos.x - bullet->width / 2),
                                static_cast<int>(bullet->pos.y - bullet->height / 2),
                                bullet->width, bullet->height};
        for(auto& enemy : enemies)
        {
            if(enemy->current_hp <= 0) continue;
            SDL_Rect rect_enemy = {static_cast<int>(enemy->pos.x - enemy->width / 2),
                                    static_cast<int>(enemy->pos.y - enemy->height / 2),
                                    enemy->width, enemy->height};
            if(SDL_HasIntersection(&rect_bullet, &rect_enemy))
            {
                enemy->current_hp -= bullet->damage;
                bullet->valid = false;
                break;
            }
        }
    }
    player_bullets.erase(std::remove_if(player_bullets.begin(), player_bullets.end(),
        [](Bullet* bullet)
        {
            bool deletable = false;
            if(!bullet->valid) 
            {
                deletable = true;
                delete bullet;
            }
            return deletable;
        }), player_bullets.end());

    // 更新敌人子弹
    for(auto& bullet : enemy_bullets)
    {
        bullet->pos += bullet->direction * bullet->speed * deltaTime;
        // 超出屏幕失效处理
        if(bullet->pos.x + bullet->width < 0 
            || bullet->pos.y + bullet->height < 0
            || bullet->pos.x > game_mgr.getWindowWidth()
            || bullet->pos.y > game_mgr.getWindowHeight())
            {
                bullet->valid = false;
                continue;
            }
        // 与玩家碰撞
        SDL_Rect rect_bullet = {static_cast<int>(bullet->pos.x - bullet->width / 2),
                                static_cast<int>(bullet->pos.y - bullet->height / 2),
                                bullet->width, bullet->height};
        SDL_Rect rect_player = {static_cast<int>(player.pos.x - player.width / 2),
                                static_cast<int>(player.pos.y - player.height / 2),
                                player.width, player.height};
        if(SDL_HasIntersection(&rect_bullet, &rect_player))
        {
            player.current_hp -= bullet->damage;
            if(player.current_hp < 0) player.current_hp = 0;
            bullet->valid = false;
            break;
        }
        
    }
    enemy_bullets.erase(std::remove_if(enemy_bullets.begin(), enemy_bullets.end(),
        [](Bullet* bullet)
        {
            bool deletable = false;
            if(!bullet->valid) 
            {
                deletable = true;
                delete bullet;
            }
            return deletable;
        }), enemy_bullets.end());
}


void SceneMain::updateProps(double deltaTime)
{
    for(auto& prop : props)
    {
        prop->pos += prop->direction * prop->speed * deltaTime;
        // 可以反弹一定次数
        if(prop->bounce_count > 0 && prop->pos.x < 0 || prop->pos.x + prop->width > game_mgr.getWindowWidth()) 
            prop->direction.x *= -1, prop->bounce_count--;
        if(prop->bounce_count > 0 && prop->pos.y < 0 || prop->pos.y + prop->width > game_mgr.getWindowHeight()) 
            prop->direction.y *= -1, prop->bounce_count--;

        // 超过反弹次数超出屏幕失效
        if(prop->bounce_count < 0 && 
        (prop->pos.x + prop->width < 0 || prop->pos.y + prop->height < 0 || 
        prop->pos.x > game_mgr.getWindowWidth() || prop->pos.y > game_mgr.getWindowHeight()))
        {
            prop->valid = false;
            continue;
        }

        // 与玩家碰撞
        SDL_Rect rect_prop = {static_cast<int>(prop->pos.x - prop->width / 2),
                            static_cast<int>(prop->pos.y - prop->height / 2),
                            prop->width, prop->height};
        SDL_Rect rect_player = {static_cast<int>(player.pos.x -player.width / 2),
                            static_cast<int>(player.pos.y -player.height / 2),
                            player.width, player.height};
        if(SDL_HasIntersection(&rect_prop, &rect_player))
        {
            score += 5;
            prop->valid = false;
            // TODO: 使用装饰模式实现玩家buff
            switch(prop->type)
            {
            case PropType::Recover: player.current_hp = std::min(player.current_hp + 1, player.max_hp); break; 
            case PropType::Shield:  break; 
            case PropType::Time:  break; 
            }
        }
    }

    props.erase(std::remove_if(props.begin(), props.end(),
    [](Prop* prop)
    {
        bool deletable = false;
        if(!prop->valid) 
        {
            deletable = true;
            delete prop;
        }
        return deletable;
    }), props.end());
}

void SceneMain::renderPlayer()
{
    SDL_Rect rect = {static_cast<int>(player.pos.x - player.width / 2), 
                    static_cast<int>(player.pos.y - player.height / 2), 
                    player.width, player.height};
    SDL_RenderCopy(game_mgr.getInstance().getRenderer(), player.tex, nullptr, &rect);
}

void SceneMain::renderEnemies()
{
    for(auto& enemy : enemies)
    {
        SDL_Rect rect = {static_cast<int>(enemy->pos.x - enemy->width / 2),
                        static_cast<int>(enemy->pos.y - enemy->height / 2),
                        enemy->width, enemy->height};
        SDL_RenderCopy(game_mgr.getRenderer(), enemy->tex, nullptr, &rect);
    }
}

void SceneMain::renderBullets()
{
    for(auto& bullet : player_bullets)
    {
        SDL_Rect rect = {static_cast<int>(bullet->pos.x - bullet->width / 2),
                        static_cast<int>(bullet->pos.y - bullet->height / 2),
                        bullet->width, bullet->height};
        SDL_RenderCopy(game_mgr.getRenderer(), bullet->tex, nullptr, &rect);
    }

    for(auto& bullet : enemy_bullets)
    {
        SDL_Rect rect = {static_cast<int>(bullet->pos.x - bullet->width / 2),
                        static_cast<int>(bullet->pos.y - bullet->height / 2),
                        bullet->width, bullet->height};
        // 图片初始有角度 
        SDL_RenderCopyEx(game_mgr.getRenderer(), bullet->tex, nullptr, &rect, bullet->direction.angle() - 90, 
        nullptr,SDL_FLIP_NONE);
    }
}

void SceneMain::renderProps()
{
    for(auto& prop : props)
    {
        SDL_Rect rect = {static_cast<int>(prop->pos.x - prop->width / 2),
                        static_cast<int>(prop->pos.y - prop->height / 2),
                        prop->width, prop->height};
        SDL_RenderCopy(game_mgr.getRenderer(), prop->tex, nullptr, &rect);
    }
}

void SceneMain::renderExplode()
{
    for(auto iter = explosion.begin(); iter != explosion.end(); )
    {
        auto& anim = *iter;
        if(anim->frame_idx >= anim->total_frame)
        {
            delete anim;
            iter = explosion.erase(iter);
        }
        else
        {
            SDL_Rect rect_src = {anim->frame_idx * anim->width, 0,
                anim->width, anim->height};
            SDL_Rect rect_dst = {static_cast<int>(anim->pos.x - anim->width / 2),
                            static_cast<int>(anim->pos.y - anim->height / 2),
                            static_cast<int>(anim->width * anim->scale), 
                            static_cast<int>(anim->height * anim->scale)};
            SDL_RenderCopy(game_mgr.getRenderer(), anim->tex, &rect_src, &rect_dst);
            anim->frame_idx++;
            ++iter;
        }
    }
}

void SceneMain::renderUI()
{
    // 玩家生命值
    static SDL_Texture* tex = ResMgr::getInstance().find_texture(ResID::Tex_HealthUIBlack);
    int w = 0, h = 0;
    if(w == 0 && h == 0)
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    static SDL_Point point = {static_cast<int>(static_cast<double>(game_mgr.getWindowWidth()) * 0.01), 
                              static_cast<int>(static_cast<double>(game_mgr.getWindowHeight()) * 0.01)};

    SDL_SetTextureColorMod(tex, 100, 100, 100);
    for(int i = 0; i < player.max_hp; ++i)
    {
        SDL_Rect rect_ui = {point.x + (w + 10) * i, point.y, w, h};
        SDL_RenderCopy(game_mgr.getRenderer(), tex, nullptr, &rect_ui);
    }
    SDL_SetTextureColorMod(tex, 255, 255, 255);
    for(int i = 0; i < player.current_hp; ++i)
    {
        SDL_Rect rect_ui = {point.x + (w + 10) * i, point.y, w, h};
        SDL_RenderCopy(game_mgr.getRenderer(), tex, nullptr, &rect_ui);
    }

    // 玩家得分
    static TTF_Font* font = ResMgr::getInstance().find_font(ResID::Font_VonwaonBitmap12px, 24);
    GameMgr::getInstance().renderText(font, "Score:" + std::to_string(score), 0.01, 0.01, false);
}


void SceneMain::spawnEnemy()
{
    if(distribution(random_generator) < 1.0 / 50.0)
    {
        auto enemy = new Enemy(enemy_template);
        enemy->pos.y = -enemy->height;
        enemy->pos.x = (game_mgr.getWindowWidth() - enemy->width) * distribution(random_generator);
        enemies.push_back(enemy);
    }
}

void SceneMain::spawnPlayerBullet()
{
    Bullet* bullet = new Bullet(player_bullet_template);
    bullet->pos = player.pos;
    player_bullets.push_back(bullet);
}


void SceneMain::spawnProp(const Vector2& pos)
{
    if(distribution(random_generator) < 0.1)
    {
        auto prop = new Prop(recover_prop_template);
        prop->pos = pos;
        prop->direction = Vector2(distribution(random_generator) * 2 * 3.1415962);
        props.push_back(prop);
    }
}