#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

class CollideBox;
class CollideMgr;
class QuadTreeNode
{
    friend class QuadTree;
private:
    SDL_FRect rect_ = { 0 };                // 所在的矩形区域
    std::vector<CollideBox*> colliders_;    // 矩形区域的物体,碰撞盒子
    QuadTreeNode* next_[4] = {nullptr};     // 子节点

public:
    QuadTreeNode(const SDL_FRect& rect):rect_(rect) {}
    ~QuadTreeNode(){};

    bool isLeaf();
    bool hasIntersection(const CollideBox* box);
    bool split(float grid_min_w, float grid_min_h);  // 空间细分
};


// 空间四叉树
class QuadTree
{
private:
    int max_node_count_ = 5;                                // 每个区域最多容纳物体个数，超过就细分区间
    glm::vec2 min_grid_size_ = glm::vec2(400.f, 400.f);     // 格子细分的最小尺寸
    QuadTreeNode* root_ = nullptr;                          // 根节点
    std::unordered_map<CollideBox*, int> colliders_refcount_;     // 注册的碰撞盒子
    std::vector<CollideBox*> colliders_to_insert_;          // 临时容器，准备下一次插入
    std::unordered_set<long long> collide_done_set_;        // 避免重复碰撞，标记已经碰撞的集合TODO:有hash冲突

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

    void checkAndProcessCollide();              // 处理碰撞，递归每一个小区间
    void clear();                               // 删除旧节点，重建新节点
    // TODO:获取四周临近的碰撞盒子,不重建树的情况下位置更新
    void debug_print(QuadTreeNode* node, int deep, const std::string& desc);
};



#endif // _QUAD_TREE_H_