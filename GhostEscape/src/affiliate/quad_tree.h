#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

// 四叉树节点
class CollideBox;
class QuadTreeNode
{
    friend class QuadTree;
private:
    SDL_FRect in_rect_ = { 0 };             // 入口边界所在的矩形区域
    SDL_FRect out_rect_ = { 0 };            // 出口边界所在的矩形区域
    std::vector<CollideBox*> colliders_;    // 矩形区域的物体,碰撞盒子
    QuadTreeNode* next_[4] = {nullptr};     // 子节点

public:
    QuadTreeNode(const SDL_FRect& rect);
    ~QuadTreeNode(){};

    bool isLeaf();
    bool hasInRectIntersection(const CollideBox* box);        // 碰撞盒和是否与入口边界区域相交
    bool hasOutRectIntersection(const CollideBox* box);       // 碰撞盒和是否与出口边界区域相交
    bool split(float grid_min_w, float grid_min_h);           // 空间细分
private:
    bool hasIntersection(const SDL_FRect& rect, const CollideBox* box);
};


// 空间四叉树   
// TODO: 还有bug，引用计数不正确、超出世界无法插入直接移除等……不过，代码能跑了。
// BUG: 因为插入失败等原因，可能导致应该碰撞，但是不发生碰撞的情况。
// 每次清理重构可以减缓这个bug，而动态更新会导致这个bug更明显
class QuadTree
{
private:
    int max_node_count_ = 5;                                        // 每个区域最多容纳物体个数，超过就细分区间
    glm::vec2 min_grid_size_ = glm::vec2(400.f, 400.f);             // 格子细分的最小尺寸,这里也可以改为树的最大深度
    QuadTreeNode* root_ = nullptr;                                  // 根节点
    std::unordered_map<CollideBox*, int> colliders_refcount_;       // 注册的碰撞盒子
    std::vector<CollideBox*> colliders_to_insert_;                  // 临时容器，准备下一次插入
    std::unordered_set<long long> collide_done_set_;                // v.01碰撞检测:避免重复碰撞，标记已经碰撞的集合TODO:小概率可能有hash冲突

public:
    QuadTree(const SDL_FRect& init_rect) { root_ = new QuadTreeNode(init_rect); }
    ~QuadTree();

    void update(float dt);                      // 更新四叉树，检测并处理碰撞
    void safeInsert(CollideBox* collide_box);   // 注册碰撞盒子
    bool erase(CollideBox* collide_box);        // 移除注册，并标记延迟删除碰撞盒子delete
    void render();                              // debug渲染

    // setters and getters
    int getMaxNodeCount() const { return max_node_count_; }
    void setMaxNodeCount(int val) { max_node_count_ = val; }
    void setMinGridSize(const glm::vec2& min_grid_size) { min_grid_size_ = min_grid_size; }
    const glm::vec2& getMinGridSize() const { return min_grid_size_; }

private:
    void destroyTreeNode(QuadTreeNode*& node);
    bool insert(CollideBox* collide_box);       
    bool _insert(QuadTreeNode* node, CollideBox* collide_box); 
    bool _erase(QuadTreeNode* node, CollideBox* collide_box);
    void _checkAndProcessCollide(QuadTreeNode* node);
    void _render(QuadTreeNode* node);
    void _clear(QuadTreeNode*& node);

    void checkAndProcessCollide();              // 处理碰撞，递归每一个小区间，然后两层for (缺点，需要检测是否是重复碰撞对)
    void checkAndProcessCollide2();             // 处理碰撞2，获取与每个碰撞盒子可能相交的盒子列表，然后检测 (天然避免了重复碰撞对)
    void clear();                               // 删除旧节点，重建新节点
    void debugPrint(QuadTreeNode* node, int deep, const std::string& desc, bool print_first = false);
    void retrieve(QuadTreeNode* node, CollideBox* box, std::unordered_set<CollideBox*>& result);   // 获取在同一分割区间的碰撞盒子
    void updateCollidersRegion(QuadTreeNode* node);         // 更新碰撞盒子所在区间
    size_t reblanceTree(QuadTreeNode* node);                // 动态收缩，当子区间拥有碰撞盒子数量少于阈值，释放空间合并节点
};


#endif // _QUAD_TREE_H_