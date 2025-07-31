#ifndef _COLLIDE_MANAGER_H_
#define _COLLIDE_MANAGER_H_

#include "../core/object.h"
#include <vector>
#include <unordered_map>

class CollideBox;
class QuadTree;

// #define COLLIDE_MGR_VERSION_QUADTREE_    // 是否启用四叉树碰撞检测版本


// (纯功能组件)这里碰撞管理器挂载到场景
// 四叉树bug暂时无从下手，如果遇到两层for不能解决的，可以考虑使用网格分区
class CollideMgr : public Object
{
private:
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    QuadTree* quad_tree_ = nullptr;     // 空间四叉树 (还有bug)
#else
    std::vector<CollideBox*> colliders_;
    std::vector<CollideBox*> colliders_to_insert_;
#endif  // COLLIDE_MGR_VERSION_QUADTREE_

    float timer_collide_ = 0.f;         // 碰撞检测定时器
    float time_collide_cd_ = 0.01f;     // 碰撞检测间隔(如果小于帧间隔则每帧都检测)
public:
    CollideMgr(Object* parent):Object(parent){}
    ~CollideMgr() override;
    virtual void update(float dt) override;     // 检测碰撞
    virtual void render() override;             // 渲染四叉树区域网格
    
    // 工具函数
    void registerCollideBox(CollideBox* box);
    void detachCollideBox(CollideBox* box);
    void reinit();    // 构建当前场景的空间四叉树
    void clean();     // 清理
    void checkAndProcessCollide2For();

    // setters and getters
    float getCheckCollideInterval() const { return time_collide_cd_; }
    void setCheckCollideInterval(float second) { time_collide_cd_ = second; }
};


#endif //_COLLIDE_MANAGER_H_