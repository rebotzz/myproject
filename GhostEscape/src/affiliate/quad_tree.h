#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "SDL3/SDL.h"
#include <vector>

class CollideBox;
class QuadTree;
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
    // QuadTreeNode() = default;
    QuadTreeNode(const SDL_FRect& rect):rect_(rect) {}
    // ~QuadTreeNode()

    bool isLeaf();
    bool isContainCollideBox(const CollideBox* box);
    bool hasIntersection(const CollideBox* box);
    bool subdevide(float grid_min_w, float grid_min_h);  // 分裂空间细分
};


// 空间四叉树
class QuadTree
{
private:
    int max_node_count_ = 30;       // 每个区域最多容纳物体个数，超过就细分区间
    float grid_min_w_ = 50.f;
    float grid_min_h_ = 50.f;
    QuadTreeNode* root_ = nullptr;

public:
    QuadTree(const SDL_FRect& init_rect) { root_ = new QuadTreeNode(init_rect); }
    ~QuadTree();

    bool insert(const CollideBox *collide_box);
    bool remove(const CollideBox *collide_box);

    // 获取四周临近的碰撞盒子,需要吗？

    // 处理碰撞，递归每一个小区间
    void processCollide();

private:
    void destroy(QuadTreeNode* node);
    bool _insert(QuadTreeNode* node, const CollideBox *box);
    bool _remove(QuadTreeNode* node, const CollideBox *collide_box);
    void _processCollide(QuadTreeNode* node);


};



#endif // _QUAD_TREE_H_