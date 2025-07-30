#ifndef _SPELL_H_
#define _SPELL_H_

#include "../core/object_world.h"
#include "../affiliate/collide_box.h"
#include "../affiliate/sprite_anim.h"

// 法术类
// 继承自世界对象，生成后属于场景组件，位置不变
class Spell : public ObjectWorld
{
protected:
    CollideBox* collide_box_ = nullptr;
    SpriteAnim* anim_ = nullptr;
    float damage_ = 20.0f;

public:
    // 父节点是具体场景
    Spell(Object* parent, float damage, const glm::vec2& target_position, CollideShape shape, ResID tex_id, 
        int frame_count, const glm::vec2& scale = glm::vec2(1), float frame_interval = 0.1f, AchorMode mode = AchorMode::CENTER);
    ~Spell() = default;

    virtual void update(float dt);
    virtual Spell* clone() const override;      // 原型模式

    // getters and setters
    CollideBox* getCollideBox() const { return collide_box_; }
    SpriteAnim* getSpriteAnim() const { return anim_; }
    float getDamage() const { return damage_; }
    void setHitCollideLayer(CollideLayer layer);
    void setHurtCollideLayer(CollideLayer layer);
    virtual void setActive(bool flag) override;

protected:
    // 数据更新
    virtual void move(float ) {};
    virtual void checkAndRemove();
};




#endif // _SPELL_H_