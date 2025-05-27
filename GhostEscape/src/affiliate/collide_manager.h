#ifndef _COLLIDE_MANAGER_H_
#define _COLLIDE_MANAGER_H_


#include "../core/object.h"
#include "../core/observer.h"
#include <memory>
#include <unordered_map>

class CollideBox;
class CollideBoxWrapper;
class QuadTree;

// 这里碰撞管理器挂载到场景
class CollideMgr : public Object, public Subject
{
    friend class QuadTree;
private:
    QuadTree* quad_tree_ = nullptr;     // 空间四叉树
    float timer_collide_ = 0.f;
    float time_collide_cd_ = 0.02f;     // 碰撞检测间隔
    std::unordered_map<CollideBox*, CollideBoxWrapper*> register_wrappers_;

public:
    CollideMgr(Object* parent):Object(parent){}
    ~CollideMgr() override;
    virtual void update(float dt) override;     // 检测碰撞
    virtual void render() override;             // debug渲染四叉网格
    
    // 工具函数
    void insertCollideBox(CollideBox* box);
    void eraseCollideBox(CollideBox* box);
    void reinit();    // 构建当前场景的空间四叉树
    void clean();     // 清理

    // 工厂方法
    CollideBox* createAndInsertCollideBox(Object *parent, CollideShape shape, const glm::vec2 &size,
                const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);
private:
    void notifyCollideBoxIsdeleted(CollideBox* box);
};


#endif //_COLLIDE_MANAGER_H_