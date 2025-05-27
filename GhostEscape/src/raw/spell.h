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
    CollideBoxWrapper* collide_box_ = nullptr;
    SpriteAnim* anim_ = nullptr;
    float damage_ = 20.0f;
    ObjectType attack_target_type_ = ObjectType::Enemy; // TODO 修改为碰撞层

public:
    // 父节点是具体场景
    Spell(Object* parent, float damage, const glm::vec2& target_position, CollideShape shape, ResID tex_id, 
        int frame_count, float scale = 1.0, float frame_interval = 0.1f, AchorMode mode = AchorMode::CENTER);
    ~Spell() {SDL_Log("~spell: box_ptr: %p", collide_box_->getInternalCollideBox()); };
 
    virtual void update(float dt);

    // getters and setters
    CollideBoxWrapper* getCollideBox() const { return collide_box_; }
    SpriteAnim* getSpriteAnim() const { return anim_; }
    void setAttackTargetType(ObjectType type) { attack_target_type_ = type; }
    ObjectType getAttackTargetType() const { return attack_target_type_; }
    float getDamage() const { return damage_; }

protected:
    // 数据更新
    virtual void updateCollide();
    virtual void move(float ) {};
    virtual void checkAndRemove();
};




#endif // _SPELL_H_