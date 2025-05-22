#ifndef _SPELL_THUNDER_H_
#define _SPELL_THUNDER_H_

#include "raw/spell.h"

class SpellThunder : public Spell
{
public:
    SpellThunder(Object* parent, float damage, const glm::vec2& target)
        :Spell(parent, damage, target, CollideShape::Circle, ResID::Tex_ThunderstrikeWBlur, 13, 3.0, 0.1f)
    { game_.playSound(ResID::Sound_BigThunder); }
    ~SpellThunder() = default;
};

#endif // _SPELL_THUNDER_H_