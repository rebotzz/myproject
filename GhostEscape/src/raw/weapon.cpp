#include "weapon.h"
#include "../core/collide_box.h"


Weapon::Weapon(float damage, float attack_cd):damage_(damage), attack_cd_(attack_cd)
{
    collide_box_ = CollideBox::createAndAddCollideBoxChild(this, CollideShape::Circle, glm::vec2(10));
    collide_box_->setActive(false);
}

void Weapon::update(float dt)
{
    Object::update(dt);
    attack_timer_ += dt;
}

void Weapon::enableAttack()
{
    enable_ = true;
    attack_timer_ = 0.0f;
    // 开启后能否遍历所有，是在持有武器对象内部遍历吗？是的
    // 在handleEvent内开启，update能遍历所有，在player内部攻击结束关闭
    collide_box_->setActive(true);      
}
