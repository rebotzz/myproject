#ifndef _SPAWNER_H_
#define _SPAWNER_H_


#include "../core/object_world.h"
#include "../core/scene.h"
#include <vector>

// v0.1生成器
// template<class T>
// class Spawner : public Object
// {
// private:
//     int count_ = 0;
//     glm::vec2 start_position_ = glm::vec2(0);
//     glm::vec2 end_position_ = glm::vec2(0);
// public:
//     Spawner(int count, const glm::vec2& start_position, const glm::vec2& end_position)
//         :count_(count), start_position_(start_position), end_position_(end_position)
//     {}
//     ~Spawner() {};

//     void spawnAndAddChild(Object* parent)
//     {
//         for(int i = 0; i < count_; ++i)
//         {
//             auto pos = game_.getRandomVec2(start_position_, end_position_);
//             new T(parent, pos);     // 构造函数完成了节点挂载,虽然看起来像是内存泄漏
//         }
//     }

//     void spawnAndAddChild(T copy_template, Object* parent)
//     {
//         for(int i = 0; i < count_; ++i)
//         {
//             auto pos = game_.getRandomVec2(start_position_, end_position_);
//             auto child = dynamic_cast<ObjectWorld*>(new T(copy_template)); 
//             child->setParent(parent);
//             child->setPosition(pos);
//         }
//     }

//     static void spawnAndAddChild(Object* parent, int count)
//     {
//         for(int i = 0; i < count; ++i)
//         {
//             auto pos = Game::getInstance().getRandomVec2(start_position_, end_position_);
//             new T(parent, pos);     // 构造函数完成了节点挂载,虽然看起来像是内存泄漏
//         }
//     }
// };


// v0.2敌人生成器
class Player;
class EnemySpawner : public Object
{
private:
    glm::ivec2 num_scope_ = glm::ivec2(1, 3);       // 生成数量区间
    glm::vec2 interval_scope_ = glm::vec2(4, 6);    // 生成间隔时间区间
    float interval_ = 4.f;                          // 生成间隔
    float timer_ = 0.f;                             // 定时器
    Player* target_ = nullptr;                      // 敌人目标

public:
    EnemySpawner(Object* parent, Player* target, glm::ivec2 num, glm::vec2 interval)
        :Object(parent), target_(target), num_scope_(num), interval_scope_(interval){}

    virtual void update(float dt) override;

    void setNumScope(glm::ivec2 val) { num_scope_ = val; }
    void setIntervalScope(glm::vec2 val) { interval_scope_ = val; }
    void setTarget(Player* target) { target_ = target; }
    glm::ivec2 getNumScope() const { return num_scope_; }
    glm::vec2 getIntervalScope() const { return interval_scope_; }
    Player* getTarget() const { return target_; }
};


#endif // _SPAWNER_H_