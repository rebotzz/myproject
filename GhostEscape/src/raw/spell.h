#ifndef _SPELL_H_
#define _SPELL_H_

#include "../core/object_world.h"
#include "../core/collide_box.h"
#include "../core/sprite_anim.h"

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
    Spell(Object* parent, float damage, const glm::vec2& target, CollideShape shape, ResID tex_id, int frame_count, float scale = 1.0, float frame_interval = 0.1f, AchorMode mode = AchorMode::CENTER);
    ~Spell() = default;
 
    virtual void update(float dt);

    // 数据更新
    void updateCollide();

    // getters and setters
    CollideBox* getCollideBox() const { return collide_box_; }
    SpriteAnim* getSpriteAnim() const { return anim_; }
};




#endif // _SPELL_H_