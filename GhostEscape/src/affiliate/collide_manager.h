#ifndef _COLLIDE_MANAGER_H_
#define _COLLIDE_MANAGER_H_


#include "../core/object.h"
#include <memory>

class CollideBox;
class QuadTree;

// 虽然基层自object但是应该挂载到那里： 场景、游戏管理器？ 场景更合理一些
class CollideMgr : public Object
{
private:
    // 空间四叉树
    QuadTree* quad_tree_ = nullptr;


public:
    CollideMgr(Object* parent):Object(parent){}
    ~CollideMgr() override;
    virtual void update(float dt) override;     // 检测碰撞
    virtual void render() override;             // debug渲染四叉网格
    
    void insertCollideBox(CollideBox* box);
    void eraseCollideBox(CollideBox* box);

    void reinit();    // 构建当前场景的空间四叉树
    void clean();   // 清理

    void removeInvalid();

    // 工厂方法
    CollideBox* createAndInsertCollideBox(Object *parent, CollideShape shape, const glm::vec2 &size,
                const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);
};


#endif //_COLLIDE_MANAGER_H_