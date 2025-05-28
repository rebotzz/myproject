#include "quad_tree.h"
#include "collide_box.h"
#include "../core/scene.h"
#include "collide_manager.h"
#include <memory>
#include <iostream>

QuadTree::~QuadTree()
{
    destroyTreeNode(root_);
    colliders_refcount_.clear();
    colliders_to_insert_.clear();
}

void QuadTree::destroyTreeNode(QuadTreeNode *&node)
{
    if(!node) return;
    // 不是叶子节点，先递归叶子节点
    if(!node->isLeaf())
    {
        for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
        {
            destroyTreeNode(node->next_[i]);
        }
    }
    // 递归结束，变成叶子节点：清理区域物体，因为一个碰撞盒子可能会跨越多个区域，使用引用计数判断是否被引用
    for(auto box : node->colliders_) 
    {
        if(box->getCanRemove()) continue;
        --colliders_refcount_[box];
    }
    node->colliders_.clear();
    // 删除节点
    delete node;
    node = nullptr;
}

void QuadTree::debug_print(QuadTreeNode* node, int deep, const std::string& desc)
{
    if(!node) return;
    std::cout << "deep: " << deep << " desc: " << desc << " box_count: " << node->colliders_.size() << std::endl;
    if(node->colliders_.size())
    {
        std::cout << "box_list: ";
        for(auto& ptr : node->colliders_)
        {
            std::cout << ptr << "\t";
        }
        std::cout << std::endl;
    }

    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        debug_print(node->next_[i], deep + 1, "region_" + std::to_string(i));
    }
}



void QuadTree::update(float dt)
{
    // 加入待添加碰撞盒子
    if(!colliders_to_insert_.empty())
    {
        for(auto& box_ptr : colliders_to_insert_)
        {
            insert(box_ptr);
        }
        colliders_to_insert_.clear();
    }
    // 碰撞盒子位置更新
    for(auto& [collider, count] : colliders_refcount_)
    {
       collider->update(dt);
    }
    // 重构四叉树（更新物体所在区域）
    clear();
    for(auto& [box_ptr, count] : colliders_refcount_)
    {
        _insert(root_, box_ptr);
    }
    // 处理碰撞
    checkAndProcessCollide();

//#ifdef DEBUG_MODE
//        // debug
//        static int idx = 0;
//        if (++idx % 10 == 0)
//        {
//            std::cout << "\n\n----------------quad_tree----------------begin:\n";
//            std::cout << "box_refcount:\n";
//            for (auto iter : box_refcount_)
//            {
//                std::cout << "[ptr:" << (iter.first) << ", cnt:" << (iter.second) << "]" << std::endl;
//            }
//            debug_print(root_, 0, "root");
//            std::cout << "----------------quad_tree----------------end!\n\n";
//        }
//#endif
}

void QuadTree::clear()
{
    auto old_rect = root_->rect_;
    _clear(root_);
    root_ = new QuadTreeNode(old_rect);
}


void QuadTree::_clear(QuadTreeNode *& node)
{
    if(!node) return;
    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        _clear(node->next_[i]);
    }

    // 到了叶子节点：清理区域碰撞盒子，删除节点，置空Node指针
    for(auto ptr : node->colliders_)
    {
        // debug: 待移除的标记前已经减少了引用计数，这里再减少就变成负数了。 to update comments
        if (ptr->getCanRemove()) continue;  
        --colliders_refcount_[ptr];
    }    
    node->colliders_.clear();
    delete node;
    node = nullptr;
}


void QuadTree::safeInsert(CollideBox* collide_box)
{
    if(colliders_refcount_.count(collide_box)) return;
    if(std::find(colliders_to_insert_.begin(), colliders_to_insert_.end(), collide_box) != colliders_to_insert_.end()) return;
    colliders_to_insert_.push_back(collide_box);
}

bool QuadTree::insert(CollideBox* collide_box)
{
    // 这是首次插入，初始化引用计数
    if(colliders_refcount_.count(collide_box)) return false;
    colliders_refcount_[collide_box] = 0;
    if(!_insert(root_, collide_box))
    {
        colliders_refcount_.erase(collide_box);
        return false;
    }
    return true;
}

bool QuadTree::_insert(QuadTreeNode *node, CollideBox* collide_box)
{
    assert(node != nullptr);
    if(!node->hasIntersection(collide_box)) return false;

    // 不是叶子节点，到叶子节点插入
    if(!node->isLeaf())
    {
        bool ret = false;
        for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
        {
            if(_insert(node->next_[i], collide_box)) ret = true;
        }
        return ret;
    }
    // 当前是叶子,只有叶子存储物体
    else
    {
        // 如果已经存在
        if(std::find(node->colliders_.begin(), node->colliders_.end(), collide_box) != node->colliders_.end())
        {
            return true;
        }
        node->colliders_.emplace_back(collide_box);
        ++colliders_refcount_[collide_box];
        // 超过最大容量，细分区间，重新分配物体所在区间
        if(node->colliders_.size() > max_node_count_ && node->split(min_grid_size_.x, min_grid_size_.y))
        {
            // 如果与多个区域重叠，那每个区域都记录
            // v0.1 插入新开辟的子区间（缺点：物体可能移动后一个都不相交，智能指针释放）
            // Debug:关键，由于物体移动，可以移除了边界，所以每个子区域都可能插入失败:这只能应对不能动的物体
            // v0.2 从根节点找合适的位置插入    此时非叶子节点，不会插入node
            for(auto& old_box : node->colliders_)
            {
                --colliders_refcount_[old_box];
                if(!_insert(root_, old_box))
                {
                    // 这里需要处理插入失败的情况，为什么会插入失败？
                    // 一般来说不会插入失败，除非物体脱离了世界边界，那么就应该移除物体。子弹？
                    SDL_Log("re insert failed: %p", old_box);
                }
            }
            node->colliders_.clear();
        }
        return true;
    }
}

bool QuadTree::erase(CollideBox* collide_box)
{
    // 这里不能用智能指针，因为shared_ptr只管理拷贝于自生的，新建的智能指针是一个单独的个体，
    // 与原来的没有关联，虽然指向同一对象地址：这会导致delete两次报错。
    // if(box_refcount_.count(collide_box) == 0) return false;

    // 没有找到，可能物体超过世界边界，碰撞管理器将其移除了
    SDL_Log("[before] erase box, ptr: %p, refcount: %d\n", collide_box, colliders_refcount_[collide_box]);
    _erase(root_, collide_box);
    SDL_Log("[after] erase box, ptr: %p, refcount: %d\n", collide_box, colliders_refcount_[collide_box]);
    colliders_refcount_.erase(collide_box);
    colliders_to_insert_.erase(std::remove(colliders_to_insert_.begin(), colliders_to_insert_.end(), collide_box), colliders_to_insert_.end());

    return true;
}

bool QuadTree::_erase(QuadTreeNode *node, CollideBox* collide_box)
{
    if(!node->hasIntersection(collide_box)) return false;

    if(!node->isLeaf())
    {
        for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
        {
            _erase(node->next_[i], collide_box);
        }
    }
    else
    {
        node->colliders_.erase(std::remove_if(node->colliders_.begin(), node->colliders_.end(), 
        [this, collide_box](CollideBox* box)
        {
            if(box == collide_box)
            {
                colliders_refcount_[box]--;   // 引用计数减少,不然最后不移除会野指针
                return true;
            }
            else return false;
        }), node->colliders_.end());
    }

    return true;
}


void QuadTree::checkAndProcessCollide()
{
    // 重置已触发碰撞标记
    collide_done_set_.clear();
    _checkAndProcessCollide(root_);
}

void QuadTree::_checkAndProcessCollide(QuadTreeNode *node)
{
    if(!node) return;

    if(!node->isLeaf())
    {
        for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
        {
            _checkAndProcessCollide(node->next_[i]);
        }
    }
    else 
    {
        // 非叶子节点不存储对象，所以到叶子节点检测碰撞
        for(size_t src_idx = 0; src_idx < node->colliders_.size(); ++src_idx)
        {
            auto& src_box = node->colliders_[src_idx];
            if(src_box->getHitLayer() == CollideLayer::None || !src_box->getIsActive()
                || src_box->getCanRemove()) continue;
            for(size_t dst_idx = 0; dst_idx < node->colliders_.size(); ++dst_idx) // 之前的受击者这次可能作为攻击者，所以可能碰撞两次,A->B,B->A
            {
                auto& dst_box = node->colliders_[dst_idx];
                if(src_box == dst_box || src_box->getHitLayer() != dst_box->getHurtLayer()
                  || !dst_box->getIsActive() || dst_box->getCanRemove()) continue;

                // 避免已经碰撞了的碰撞对, TODO修改hash函数，避免hash冲突
                long long hash_key = reinterpret_cast<long long>(src_box) + reinterpret_cast<long long>(dst_box);
                if(collide_done_set_.find(hash_key) != collide_done_set_.end()) continue;

                if(src_box->checkCollision(dst_box))
                {
                    collide_done_set_.insert(hash_key); // 标记已经碰撞
                    // Bug:为什么这里的回调会触发上千次? 
                    // Debug:已经触发的碰撞对盒子再次触发，因为碰撞盒子体积可能跨越多个区域，解决：collide_done_set_检测已经碰撞了的。
                    src_box->processCollide(dst_box);
                    dst_box->processCollide(src_box);
                }
            }
        }
    }
}

void QuadTree::render()
{
    _render(root_);
}



void QuadTree::_render(QuadTreeNode* node)
{
    if(!node) return;
    // 渲染自身区域网格，如果子区域存在，递归渲染. 其实不用每个都判断，子区域是一起创建一同销毁的。

    // 这里可能需要转化为渲染坐标
    auto pos = Game::getInstance().getCurrentScene()->worldToScreen({node->rect_.x, node->rect_.y});
    SDL_FRect render_rect = {pos.x, pos.y, node->rect_.w, node->rect_.h};
    Game::getInstance().renderRect(render_rect, {1.0f, 0.0f, 0.0f, 1.f});
    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        _render(node->next_[i]);
    }
}

bool QuadTreeNode::isLeaf()
{
    for(int i = 0; i < sizeof(next_) / sizeof(QuadTreeNode*); ++i)
    {
        if(next_[i] != nullptr) return false;
    }
    return true;
}


bool QuadTreeNode::hasIntersection(const CollideBox *box)
{
    bool intersect = false;
    if(CollideShape::Rectangle == box->getCollideShape())
    {
        auto top_left = box->getPosition() + box->getOffset();
        auto size = box->getSize();
        SDL_FRect rect_box = {top_left.x, top_left.y, size.x, size.y};
        if(SDL_HasRectIntersectionFloat(&rect_box, &rect_))
            intersect = true;
    }
    else if(CollideShape::Circle == box->getCollideShape())
    {
        if(CollideBox::checkIntersectRectCircle(glm::vec2(rect_.x, rect_.y), {rect_.w, rect_.h}, 
        box->getPosition(), box->getSize().x))
        {
            intersect = true;
        }
    }

    return intersect;
}

bool QuadTreeNode::split(float min_grid_w, float min_grid_h)
{    
    if(rect_.w <= min_grid_w && rect_.h <= min_grid_h) return false;

    float sub_w = rect_.w / 2.f, sub_h = rect_.h / 2.f;
    for(int i = 0; i < sizeof(next_) / sizeof(QuadTreeNode*); ++i)
    {
        if(nullptr == next_[i])
        {
            SDL_FRect rect = {rect_.x + static_cast<float>(i % 2) * sub_w, rect_.y + static_cast<float>(i / 2) * sub_h, sub_w, sub_h};
            next_[i] = new QuadTreeNode(rect);
        }
    }
    return true;
}
