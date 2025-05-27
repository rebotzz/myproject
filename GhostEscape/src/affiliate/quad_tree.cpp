#include "quad_tree.h"
#include "collide_box.h"
#include "../core/scene.h"
#include "collide_manager.h"
#include <memory>
#include <iostream>

QuadTree::~QuadTree()
{
    destroy(root_);
    for(auto [box, count] : box_refcount_)
    {
        delete box;
    }
    for(auto box : to_insert_)
    {
        delete box;
    }
    box_refcount_.clear();
    to_insert_.clear();
}



void QuadTree::destroy(QuadTreeNode *&node)
{
    if(!node) return;
    // 不是叶子节点，先递归叶子节点
    if(!node->isLeaf())
    {
        destroy(node->top_left_);
        destroy(node->top_right_);
        destroy(node->bottom_left_);
        destroy(node->bottom_right_);
    }
    // 递归结束，变成叶子节点：清理区域物体
    // 不能直接delete,因为一个碰撞盒子可能会跨越多个区域。使用hash引用计数，或者智能指针。 
    for(auto box : node->objects_) 
    {
        if(box->getCanRemove()) continue;
        --box_refcount_[box];
    }
    node->objects_.clear();

    // 不需要上面这些，node到这里就一定是叶子,没有子区间，子区间在递归途中解决了
    delete node;
    node = nullptr;
}

void QuadTree::debug_print(QuadTreeNode* node, int deep, const std::string& desc)
{
    if(!node) return;
    std::cout << "deep: " << deep << " desc: " << desc << " box_count: " << node->objects_.size() << std::endl;
    if(node->objects_.size())
    {
        std::cout << "box_list: ";
        for(auto& ptr : node->objects_)
        {
            std::cout << ptr << "\t";
        }
        std::cout << std::endl;
    }

    debug_print(node->top_left_, deep + 1, "top_left");
    debug_print(node->top_right_, deep + 1, "top_right_");
    debug_print(node->bottom_left_, deep + 1, "bottom_left_");
    debug_print(node->bottom_right_, deep + 1, "bottom_right_");
}

void QuadTree::deleteCollideBox(CollideBox* collide_box)
{
    if(collide_mgr_)
    {
        collide_mgr_->notifyCollideBoxIsdeleted(collide_box);
    }
    delete collide_box;
}

void QuadTree::update(float dt)
{
    // 移除无效碰撞盒子。碰撞盒子位置更新
    for(auto iter = box_refcount_.begin(); iter != box_refcount_.end();)
    {
        // 需要这iter->second == 0后半句吗？如果没有持有者，那么提前删了也不不影响，但是会导致生命周期管理混乱
        if(iter->first->getCanRemove() || iter->second == 0)  
        {
            CollideBox* box_ptr = iter->first; 
            int refcount = iter->second;
            iter = box_refcount_.erase(iter);

//#ifdef DEBUG_MODE
//            SDL_Log("remove box, ptr: %p, refcount: %d\n", box_ptr, refcount);
//#endif
            if (refcount >= 0)  // 临时方案，引用计数出错了。
            {
                deleteCollideBox(box_ptr);
            }
            else
            {
                abort();
            }
        }
        else
        {
            iter->first->update(dt);
            ++iter;
        }
    }

    // 碰撞盒子位置更新
    //for(auto& [box, count] : box_refcount_)
    //{
    //    // bug：为什么这里的box会是野指针，不是移除了吗？ Debug:放在上面更新
    //    box->update(dt);
    //}

    // 加入待添加碰撞盒子
    if(!to_insert_.empty())
    {
        for(auto& box_ptr : to_insert_)
        {
            insert(box_ptr);
        }
        to_insert_.clear();
    }

    // 一定时间后重构
    timer_update_tree_ += dt;
    if(timer_update_tree_ > time_update_tree_cd_)
    {
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
        timer_update_tree_ = 0.f;
        clear();
        for(auto& [box_ptr, count] : box_refcount_)
        {
            _insert(root_, box_ptr);
        }
    }

    // 处理碰撞
    checkAndProcessCollide();
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
    _clear(node->top_left_);
    _clear(node->top_right_);
    _clear(node->bottom_left_);
    _clear(node->bottom_right_);

    for(auto ptr : node->objects_)
    {
        // debug: 待移除的标记前已经减少了引用计数，这里再减少就变成负数了。
        if (ptr->getCanRemove()) continue;  
        box_refcount_[ptr]--;
    }    
    node->objects_.clear();
    node->top_left_ = nullptr;
    node->top_right_ = nullptr;
    node->bottom_left_ = nullptr;
    node->bottom_right_ = nullptr;
    delete node;
    node = nullptr;
}


void QuadTree::safeInsert(CollideBox* collide_box)
{
    if(box_refcount_.count(collide_box)) return;
    to_insert_.push_back(collide_box);
}

bool QuadTree::insert(CollideBox* collide_box)
{
    // 这是首次插入，初始化引用计数
    if(box_refcount_.count(collide_box)) return false;
    box_refcount_[collide_box] = 0;
    if(!_insert(root_, collide_box))
    {
        deleteCollideBox(collide_box);
        box_refcount_.erase(collide_box);
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
        if(_insert(node->top_left_, collide_box)) ret = true;
        if(_insert(node->top_right_, collide_box)) ret = true;
        if(_insert(node->bottom_left_, collide_box)) ret = true;
        if(_insert(node->bottom_right_, collide_box)) ret = true;
        return ret;
    }
    // 当前是叶子,只有叶子存储物体
    else
    {
        // 如果已经存在
        if(std::find(node->objects_.begin(), node->objects_.end(), collide_box) != node->objects_.end())
        {
            return true;
        }
        node->objects_.emplace_back(collide_box);
        box_refcount_[collide_box]++;
        // 超过最大容量，细分区间，重新分配物体所在区间
        if(node->objects_.size() > max_node_count_ && node->subdevide(min_grid_size_.x, min_grid_size_.y))
        {
            // 如果与多个区域重叠，那每个区域都记录
            // v0.1 插入新开辟的子区间（缺点：物体可能移动后一个都不相交，智能指针释放）
            // Debug:关键，由于物体移动，可以移除了边界，所以每个子区域都可能插入失败:这只能应对不能动的物体
            // for(const auto& old_box : node->objects_)
            // {
            //     _insert(node->top_left_, old_box);
            //     _insert(node->top_right_, old_box);
            //     _insert(node->bottom_left_, old_box);
            //     _insert(node->bottom_right_, old_box);
            // }

            // v0.2 从根节点找合适的位置插入    // 此时非叶子节点，不会插入node
            for(auto& old_box : node->objects_)
            {
                if(_insert(root_, old_box))     // 插入成功，移除原有计数
                {
                    --box_refcount_[old_box];   
                }
                else        
                {
                    // 这里需要处理插入失败的情况，为什么会插入失败？
                    // 一般来说不会插入失败，除非物体脱离了世界边界，那么就应该移除物体。
                    // 子弹？
                    SDL_Log("re insert failed: %p", old_box);
                    //abort();
                    old_box->setCanRemove(true);  
                    --box_refcount_[old_box];   
                }
            }
            node->objects_.clear();
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
    if(box_refcount_.find(collide_box) == box_refcount_.end())
    {
        return true;
    }

    SDL_Log("[before] erase box, ptr: %p, refcount: %d\n", collide_box, box_refcount_[collide_box]);
    _erase(root_, collide_box);
    SDL_Log("[after] erase box, ptr: %p, refcount: %d\n", collide_box, box_refcount_[collide_box]);
    collide_box->setCanRemove(true);
    return true;
}

bool QuadTree::_erase(QuadTreeNode *node, CollideBox* collide_box)
{
    if(!node->hasIntersection(collide_box)) return false;

    if(!node->isLeaf())
    {
        _erase(node->top_left_, collide_box);
        _erase(node->top_right_, collide_box);
        _erase(node->bottom_left_, collide_box);
        _erase(node->bottom_right_, collide_box);
    }
    else
    {
        for(auto iter = node->objects_.begin(); iter != node->objects_.end(); )
        {
            if(*iter == collide_box)
            {
                box_refcount_[collide_box]--;   // 引用计数减少,不然最后不移除会野指针
                iter = node->objects_.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
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
        _checkAndProcessCollide(node->top_left_);
        _checkAndProcessCollide(node->top_right_);
        _checkAndProcessCollide(node->bottom_left_);
        _checkAndProcessCollide(node->bottom_right_);
    }
    else 
    {
        // 非叶子节点不存储对象，所以到叶子节点检测碰撞
        for(size_t src_idx = 0; src_idx < node->objects_.size(); ++src_idx)
        {
            auto& src_box = node->objects_[src_idx];
            if(src_box->getHitLayer() == CollideLayer::None || !src_box->getIsActive()
                || src_box->getCanRemove()) continue;
            for(size_t dst_idx = 0; dst_idx < node->objects_.size(); ++dst_idx) // 之前的受击者这次可能作为攻击者，所以可能碰撞两次
            {
                auto& dst_box = node->objects_[dst_idx];
                if(src_box == dst_box || src_box->getHitLayer() != dst_box->getHurtLayer()
                  || !dst_box->getIsActive() || dst_box->getCanRemove()) continue;

                // 避免已经碰撞了的碰撞对
                long long hash_key = reinterpret_cast<long long>(src_box) + reinterpret_cast<long long>(dst_box);
                if(collide_done_set_.find(hash_key) != collide_done_set_.end()) continue;

                if(src_box->checkCollision(dst_box))
                {
                    collide_done_set_.insert(hash_key); // 标记已经碰撞

                    // 记录相撞的碰撞盒子，执行碰撞回调
                    src_box->setOnCollideBox(dst_box);
                    dst_box->setOnCollideBox(src_box);

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
    _render(node->top_left_);
    _render(node->top_right_);
    _render(node->bottom_left_);
    _render(node->bottom_right_);
}

bool QuadTreeNode::isLeaf()
{
    return top_left_ == nullptr && top_right_ == nullptr 
        && bottom_left_ == nullptr && bottom_right_ == nullptr;
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

bool QuadTreeNode::subdevide(float min_grid_w, float min_grid_h)
{    
    if(rect_.w <= min_grid_w && rect_.h <= min_grid_h) return false;
    if(!top_left_)
    {
        top_left_ = new QuadTreeNode({rect_.x, rect_.y, rect_.w / 2.f, rect_.h / 2.f});
    }
    if(!top_right_)
    {
        top_right_ = new QuadTreeNode({rect_.x + rect_.w / 2.f, rect_.y, rect_.w / 2.f, rect_.h / 2.f});
    }
    if(!bottom_left_)
    {
        bottom_left_ = new QuadTreeNode({rect_.x, rect_.y + rect_.h / 2.f, rect_.w / 2.f, rect_.h / 2.f});
    }
    if(!bottom_right_)
    {
        bottom_right_ = new QuadTreeNode({rect_.x + rect_.w / 2.f, rect_.y + rect_.h / 2.f, rect_.w / 2.f, rect_.h / 2.f});
    }
    return true;
}
