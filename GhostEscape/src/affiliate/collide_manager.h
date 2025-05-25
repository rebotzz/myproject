#ifndef _COLLIDE_MANAGER_H_
#define _COLLIDE_MANAGER_H_

#include "../core/object.h"

class CollideBox;
class QuadTree;

class CollideMgr : public Object
{
private:
    // 空间四叉树
    QuadTree* quad_tree_ = nullptr;

public:


    virtual void update(float dt) override;     // 检测碰撞
    virtual void render() override;             // debug渲染四叉网格
    
    void insertCollideBox(CollideBox* box);
    void eraseCollideBox(CollideBox* box);

    void init();    // 构建当前场景的空间四叉树
    void clean();   // 清理
};


#endif //_COLLIDE_MANAGER_H_