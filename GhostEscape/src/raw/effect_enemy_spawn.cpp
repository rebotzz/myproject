#include "effect_enemy_spawn.h"
#include "../enemy.h"
#include "../scene_main.h"

EffectEnemySpawn::EffectEnemySpawn(Object *parent, const glm::vec2 &position)
    :Effect(parent, position, ResID::Tex_1764, 17, 3.0f)
{
    // 局部变量会失效，不能引用捕获
    setOnFinished([parent, position, this]()
    {
        auto enemy = Enemy::createAndAddEnemyChild(parent, position);
        enemy->setTarget(dynamic_cast<SceneMain*>(game_.getCurrentScene())->getPlayer());
    });
    game_.playSound(ResID::Sound_SillyGhostSound242342);
}