#include "spawner.h"
#include "../player.h"
#include "../enemy.h"
#include "../world/effect.h"

void EnemySpawner::update(float dt)
{
    timer_ += dt;
    if(timer_ >= interval_)
    {
        timer_ = 0.f;
        interval_ = game_.getRandom(interval_scope_.x, interval_scope_.y);
        if(target_ && target_->getIsActive())
        {
            int num = game_.getRandom(num_scope_.x, num_scope_.y);
            for(int i = 0; i < num; ++i)
            {
                auto current_scene = game_.getCurrentScene();
                auto position = game_.getRandomVec2(current_scene->getCameraPosition(), current_scene->getCameraPosition() + game_.getScreenSize());
                // 特效
                auto effect = new Effect(current_scene, position, ResID::Tex_1764, 17, 3.0f);
                effect->setOnFinished([current_scene, position, this]()     // 局部变量会失效，不能引用捕获
                {
                    // 敌人
                    auto enemy = new Enemy(current_scene, position);
                    enemy->setTarget(target_);
                });
                game_.playSound(ResID::Sound_SillyGhostSound242342);
            }
        }
    }
}