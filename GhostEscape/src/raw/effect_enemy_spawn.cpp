#include "effect_enemy_spawn.h"
#include "../enemy.h"

EffectEnemySpawn::EffectEnemySpawn(Object *parent, const glm::vec2 &position)
    :Effect(parent, position, ResID::Tex_1764, 17, 3.0f)
{
    // 局部变量会失效，不能引用捕获
    setOnFinished([parent, position]()
    {
        Enemy::createAndAddEnemyChild(parent, position);
    });
    game_.playSound(ResID::Sound_SillyGhostSound242342);
}