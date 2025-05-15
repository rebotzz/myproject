#include "sceneMain.h"
#include "resourcesMgr.h"
#include "vector2.h"
#include "gameMgr.h"
#include "sceneBegin.h"
#include "sceneEnd.h"
#include <algorithm>

SceneMain::SceneMain()
{
    distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    auto init_texture = [](SDL_Texture*& tex, ResID res_id, int& w, int& h, double w_scale, double h_scale)
    {
        tex = ResMgr::getInstance().find_texture(res_id);
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        w = static_cast<int>(w * w_scale);
        h = static_cast<int>(h * h_scale);
    };

    // 敌人模板初始化:敌人,敌人子弹,敌人子弹发射逻辑
    init_texture(enemy_template1.tex, ResID::Tex_Insect2, enemy_template1.width, enemy_template1.height, 0.25, 0.25);
    enemy_template1.direction = {0, 1};
    enemy_template1.current_hp = 2;

    init_texture(enemy_template2.tex, ResID::Tex_Insect1, enemy_template2.width, enemy_template2.height, 0.25, 0.25);
    enemy_template2.shoot_cd = 0.2;
    enemy_template2.current_hp = 1;
    enemy_template2.direction = {0, 1};
    enemy_template2.speed *= 0.5;

    // 为了简便,陨石也当作敌人,只不过不能发射子弹
    init_texture(enemy_template3_stone.tex, ResID::Tex_SmallA, enemy_template3_stone.width, enemy_template3_stone.height, 1.0, 1.0);
    enemy_template3_stone.shoot_cd = INT_MAX;
    enemy_template3_stone.current_hp = 3;
    enemy_template3_stone.direction = {0, 1};
    enemy_template3_stone.speed *= 0.7;

    init_texture(enemy_template4_stone.tex, ResID::Tex_SmallB, enemy_template4_stone.width, enemy_template4_stone.height, 0.5, 0.5);
    enemy_template4_stone.shoot_cd = INT_MAX;
    enemy_template4_stone.current_hp = 1;
    enemy_template4_stone.direction = {0, 1};

    auto& enemy_bullet_template1 = enemy_template1.bullet_template;
    init_texture(enemy_bullet_template1.tex, ResID::Tex_Bullet1, enemy_bullet_template1.width, enemy_bullet_template1.height, 0.25, 0.25);
    enemy_bullet_template1.damage = enemy_template1.damage;
    enemy_bullet_template1.speed *= 0.7;

    auto& enemy_bullet_template2 = enemy_template2.bullet_template;
    init_texture(enemy_bullet_template2.tex, ResID::Tex_Fire, enemy_bullet_template2.width, enemy_bullet_template2.height, 0.2, 0.2);
    enemy_bullet_template2.damage = enemy_template2.damage;
    enemy_bullet_template2.speed *= 0.3;

    // 为了确保lambda捕获字段有效: 方法1.每次enter初始化一次lambda; 方法2.改为形式参数
    enemy_template1.spawn_bullet = [](Enemy* enemy, std::vector<Bullet*>& bullets, AbstractPlayer* player)
    {
        Bullet* bullet = new Bullet(enemy->bullet_template);
        bullet->direction = (player->get_pos() - enemy->pos).normalize();
        bullet->pos = enemy->pos;
        bullets.push_back(bullet);
        Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Eff11), 0);
    };
    enemy_template2.spawn_bullet = [](Enemy* enemy, std::vector<Bullet*>& bullets, AbstractPlayer* player)
    {
        const int bullet_count = 3;
        const double angle_speed = 90;
        for(int i = 0; i < bullet_count; ++i)
        {
            Bullet* bullet = new Bullet(enemy->bullet_template);
            int angle = enemy->bullet_angle + i * 360 / bullet_count;
            bullet->direction = Vector2(angle);
            bullet->pos = enemy->pos + bullet->direction * enemy->width / 2;
            bullets.push_back(bullet);
        }
        enemy->bullet_angle = (enemy->bullet_angle + static_cast<int>(angle_speed * enemy->shoot_cd)) % 360;
        Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Eff11), 0);
    };

    // 初始化爆炸动画
    explode_animtion_template.tex = ResMgr::getInstance().find_texture(ResID::Tex_Explosion);
    SDL_QueryTexture(explode_animtion_template.tex, nullptr, nullptr, &explode_animtion_template.width, &explode_animtion_template.height);
    explode_animtion_template.total_frame = explode_animtion_template.width / explode_animtion_template.height;
    explode_animtion_template.width /= explode_animtion_template.total_frame;

    // 初始化道具模板
    auto init_prop_template = [](Prop& prop_template, ResID red_id, PropType prop_type)
        {
            prop_template.tex = ResMgr::getInstance().find_texture(red_id);
            SDL_QueryTexture(prop_template.tex, nullptr, nullptr, &prop_template.width, &prop_template.height);
            prop_template.width /= 4;
            prop_template.height /= 4;
            prop_template.type = prop_type;
        };
    init_prop_template(recover_prop_template, ResID::Tex_BonusLife, PropType::Recover);
    init_prop_template(shield_prop_template, ResID::Tex_BonusShield, PropType::Shield);
    init_prop_template(time_prop_template, ResID::Tex_BonusTime, PropType::Time);
    init_prop_template(time2_prop_template, ResID::Tex_BonusTime2, PropType::Time2);
}

SceneMain::~SceneMain()
{}


void SceneMain::enter()
{
    player = new BasePlayer();
    score = 0;
    timer_end_countdown = 3.0;
    Mix_FadeInMusic(ResMgr::getInstance().find_music(ResID::Mus_RacingThroughAsteroidsLoop), -1, 300);
}

void SceneMain::exit()
{
    // 停止音乐
    Mix_FadeOutMusic(300);

    // 记录玩家得分
    game_mgr.setScore(score);

    // 清理资源
    for(auto& enemy : enemies)
        if(enemy) delete enemy;
    for(auto& bullet : enemy_bullets)
        if(bullet) delete bullet;
    for(auto& ptr : props)
        if(ptr) delete ptr;
    for(auto& ptr : explosion)
        if(ptr) delete ptr;
    enemies.clear();
    enemy_bullets.clear();
    props.clear();
    explosion.clear();

    // 包装器层层析构
    while(player)
    {
        auto internal_player = player->get_interal_player();
        delete player;
        player = internal_player;
    }
}

void SceneMain::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
        game_mgr.switchScene(new SceneBegin);
    }
}

void SceneMain::update(double deltaTime)
{
    player->update(deltaTime);
    timer_stop_time_countdown -= deltaTime;
    double scaledDeltaTime = deltaTime * (timer_stop_time_countdown > 0 ? 0 : 1);
    spawnEnemy();
    updateEnemies(scaledDeltaTime);
    updateBullets(scaledDeltaTime);
    updateProps(scaledDeltaTime);
    updateExplode(scaledDeltaTime);

    if(player->get_current_hp() <= 0)
    {
        timer_end_countdown -= deltaTime;
        if(timer_end_countdown < 0) 
        {
            game_mgr.switchScene(new SceneEnd);
            return;
        }
    }
    // Buff有效检测
    if(!player->get_wrapper_valid())
    {
        auto internal_player = player->get_interal_player();
        delete player;
        player = internal_player;
    }
}

void SceneMain::render()
{
    renderEnemies();
    player->render();
    renderBullets();
    renderProps();
    renderExplode();
    renderUI();
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
        if(enemy->current_hp > 0 && player->get_current_hp() > 0 && enemy->last_shoot_passed_time > enemy->shoot_cd)
        {
            enemy->last_shoot_passed_time = 0;
            if(enemy->spawn_bullet)
                enemy->spawn_bullet(enemy, enemy_bullets, player);
        }

        // 与玩家相撞
        SDL_Rect rect_player = {static_cast<int>(player->get_pos().x - player->get_width() / 2),
            static_cast<int>(player->get_pos().y - player->get_height() / 2),
            player->get_width(), player->get_height()};
        SDL_Rect rect_enemy = {static_cast<int>(enemy->pos.x - enemy->width / 2),
            static_cast<int>(enemy->pos.y - enemy->height / 2),
            enemy->width, enemy->height};
        if(player->get_current_hp() > 0 && SDL_HasIntersection(&rect_player, &rect_enemy))
        {
            player->decrease_hp(enemy->damage);
            player_hurt_effect();
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
                    Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Explosion3), 0);
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
    auto& player_bullets = player->get_bullets();
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
                Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Hit), 0);
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
        SDL_Rect rect_player = {static_cast<int>(player->get_pos().x - player->get_width() / 2),
                                static_cast<int>(player->get_pos().y - player->get_height() / 2),
                                player->get_width(), player->get_height()};
        if(player->get_current_hp() > 0 && SDL_HasIntersection(&rect_bullet, &rect_player))
        {
            player->decrease_hp(bullet->damage);
            player_hurt_effect();
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
        if(prop->bounce_count > 0 && (prop->pos.x < 0 || prop->pos.x + prop->width > game_mgr.getWindowWidth())) 
            prop->direction.x *= -1, prop->bounce_count--;
        if(prop->bounce_count > 0 && (prop->pos.y < 0 || prop->pos.y + prop->width > game_mgr.getWindowHeight())) 
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
        SDL_Rect rect_player = {static_cast<int>(player->get_pos().x -player->get_width() / 2),
                            static_cast<int>(player->get_pos().y -player->get_height() / 2),
                            player->get_width(), player->get_height()};
        if(player->get_current_hp() > 0 && SDL_HasIntersection(&rect_prop, &rect_player))
        {
            score += 5;
            prop->valid = false;
            Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Eff5), 0);
            // TODO: 使用装饰模式实现玩家buff
            switch(prop->type)
            {
            case PropType::Recover: player->increase_hp(1); break; 
            case PropType::Shield: player = new ShieldPlayer(player); break; 
            case PropType::Time: player = new TimePlayer(player); break; 
            case PropType::Time2: timer_stop_time_countdown = STOP_TIME; break; 
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

void SceneMain::updateExplode(double deltaTime)
{
    for(auto iter = explosion.begin(); iter != explosion.end(); )
    {
        auto& anim = *iter;
        anim->pass_time += deltaTime;
        anim->frame_idx = static_cast<int>(anim->pass_time / anim->frame_delta);
        if(anim->frame_idx >= anim->total_frame)
        {
            delete anim;
            iter = explosion.erase(iter);
        }
        else ++iter;
    }
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
    for(auto& bullet : player->get_bullets())
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
    for(auto& anim : explosion)
    {
        SDL_Rect rect_src = {anim->frame_idx * anim->width, 0,
            anim->width, anim->height};
        SDL_Rect rect_dst = {static_cast<int>(anim->pos.x - anim->width / 2),
                        static_cast<int>(anim->pos.y - anim->height / 2),
                        static_cast<int>(anim->width * anim->scale), 
                        static_cast<int>(anim->height * anim->scale)};
        SDL_RenderCopy(game_mgr.getRenderer(), anim->tex, &rect_src, &rect_dst);
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
    for(int i = 0; i < player->get_max_hp(); ++i)
    {
        SDL_Rect rect_ui = {point.x + (w + 10) * i, point.y, w, h};
        SDL_RenderCopy(game_mgr.getRenderer(), tex, nullptr, &rect_ui);
    }
    SDL_SetTextureColorMod(tex, 255, 255, 255);
    for(int i = 0; i < player->get_current_hp(); ++i)
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
    // 简单点,没用定时器,每一帧有一定概率生成敌人;目前FPS=60
    if(distribution(random_generator) < 1.0 / 30.0)
    {
        Enemy* enemy = nullptr; 
        double random_num = distribution(random_generator);
        if(random_num < 0.5) enemy = new Enemy(enemy_template1);
        else if(random_num < 0.7) enemy = new Enemy(enemy_template2);
        else if(random_num < 0.8) enemy = new Enemy(enemy_template3_stone);
        else enemy = new Enemy(enemy_template4_stone);

        enemy->pos.y = -enemy->height;
        enemy->pos.x = enemy->width / 2 + (game_mgr.getWindowWidth() - enemy->width) * distribution(random_generator);
        enemies.push_back(enemy);
    }
}

void SceneMain::spawnProp(const Vector2& pos)
{
    if(distribution(random_generator) < 0.2)
    {
        double random_num = distribution(random_generator);
        Prop* prop = nullptr;
        if(random_num < 0.4) prop = new Prop(recover_prop_template);
        else if(random_num < 0.7) prop = new Prop(shield_prop_template);
        else if(random_num < 0.9) prop = new Prop(time_prop_template);
        else prop = new Prop(time2_prop_template);
        prop->pos = pos;
        prop->direction = Vector2(distribution(random_generator) * 2 * 3.1415962);
        props.push_back(prop);
        Mix_PlayChannel(-1, ResMgr::getInstance().find_sound(ResID::Sound_Eff5), 0);
    }
}

void SceneMain::player_hurt_effect()
{
    if(player->get_current_hp() > 0)
    {
        Mix_PlayChannel(0, ResMgr::getInstance().find_sound(ResID::Sound_PlayerHurt), 0);
    }
    else
    {
        auto anim_explosion = new ExplodeAnimation(explode_animtion_template);
        anim_explosion->pos = player->get_pos();
        explosion.push_back(anim_explosion);
        Mix_PlayChannel(0, ResMgr::getInstance().find_sound(ResID::Sound_Explosion1), 0);
    }
}
