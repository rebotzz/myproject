#ifndef _EFFECT_ENEMY_SPAWN_H_
#define _EFFECT_ENEMY_SPAWN_H_

#include "effect.h"

class EffectEnemySpawn : public Effect
{
public:
    EffectEnemySpawn(Object* parent, const glm::vec2& position);
};

#endif // _EFFECT_ENEMY_SPAWN_H_