#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "SDL3/SDL.h"
#include <vector>
#include <memory>
#include <unordered_map>

class CollideBox;
class QuadTreeNode
{
    friend class QuadTree;
private:
    SDL_FRect rect_ = { 0 };                // 所在的矩形区域
    std::vector<CollideBox*> objects_;      // 矩形区域的物体,碰撞盒子
    QuadTreeNode* top_left_ = nullptr;      // 子节点
    QuadTreeNode* top_right_ = nullptr;
    QuadTreeNode* bottom_left_ = nullptr;
    QuadTreeNode* bottom_right_ = nullptr;

public:
    QuadTreeNode(const SDL_FRect& rect):rect_(rect) {}
    ~QuadTreeNode();

    bool isLeaf();
    // bool isContainCollideBox(const CollideBox* box);
    bool hasIntersection(const CollideBox* box);
    bool subdevide(float grid_min_w, float grid_min_h);  // 分裂空间细分
};


// 空间四叉树
class QuadTree
{
private:
    int max_node_count_ = 5;       // 每个区域最多容纳物体个数，超过就细分区间
    float grid_min_w_ = 200.f;
    float grid_min_h_ = 200.f;
    QuadTreeNode* root_ = nullptr;
    // std::vector<std::shared_ptr<CollideBox>> to_insert_;    // 临时容器，准备下一次插入
    // std::vector<CollideBox*> box_list_;    // 临时容器，准备下一次插入
    std::unordered_map<CollideBox*, int> box_refcount_;
    float timer_ = 0.f;
    float timer_update_cd_ = 0.1f;

public:
    QuadTree(const SDL_FRect& init_rect) { root_ = new QuadTreeNode(init_rect); }
    ~QuadTree();

    void update(float dt);
    bool insert(CollideBox* collide_box);
    bool erase(CollideBox* collide_box);

    // 获取四周临近的碰撞盒子,需要吗？
    // 位置更新
    
    // 处理碰撞，递归每一个小区间
    void checkAndProcessCollide();
    // debug渲染
    void render();

    void clear(QuadTreeNode* node);

    // setters and getters
    int getMaxNodeCount() const { return max_node_count_; }
    void setMaxNodeCount(int val) { max_node_count_ = val; }
    void setMinGridSize(int w, int h) { grid_min_w_ = w, grid_min_h_ = h; }

    void debug_print(QuadTreeNode* node, int deep);

private:
    void destroy(QuadTreeNode*& node);
    // 智能指针，可能多个区域持有同一碰撞盒子,引用计数便于管理. 不，智能指针不方便debug
    bool _insert(QuadTreeNode* node, CollideBox* collide_box); 
    bool _erase(QuadTreeNode* node, CollideBox* collide_box);
    void _checkAndProcessCollide(QuadTreeNode* node);
    void _render(QuadTreeNode* node);

};



#endif // _QUAD_TREE_H_