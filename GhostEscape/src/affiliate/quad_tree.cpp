#include "quad_tree.h"
#include "collide_box.h"
#include <memory>
#include <iostream>
#include "../core/scene.h"

QuadTree::~QuadTree()
{
    destroy(root_);
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
    // for(auto& box : node->objects_) if(box) delete box;    
    node->objects_.clear();

    // 不需要上面这些，node到这里就一定是叶子,没有子区间，子区间在递归途中解决了
    delete node;
    node = nullptr;
}

QuadTreeNode::~QuadTreeNode()
{

}

void QuadTree::debug_print(QuadTreeNode* node, int deep)
{
    if(!node) return;
    std::cout << "node_ptr: " << node << " deep: " << deep << " box_count: " << node->objects_.size() << std::endl;
    if(node->objects_.size())
    {
        std::cout << "box_list: ";
        for(auto& ptr : node->objects_)
        {
            std::cout << ptr << "\t";
        }
        std::cout << std::endl;
    }

    debug_print(node->top_left_, deep + 1);
    debug_print(node->top_right_, deep + 1);
    debug_print(node->bottom_left_, deep + 1);
    debug_print(node->bottom_right_, deep + 1);
}

void QuadTree::update(float dt)
{
    // 如果erase后，这里失效，就变成野指针了
    // if(!to_insert_.empty())
    // {
    //     // SDL_Log("to_insert_.size(): [%u]", to_insert_.size());
    //     for(auto& box : to_insert_)
    //     {
    //         // SDL_Log("[before]to_insert_shared_ptr [%p] use_count: %d", box.get(), box.use_count());
    //         _insert(root_, box);
    //         // SDL_Log("[after]to_insert_shared_ptr [%p] use_count: %d", box.get(), box.use_count());
    //     }
    //     to_insert_.clear();
    // }

    // debug
    // static size_t old_size = box_list_.size();
    // if(old_size != box_list_.size())
    // {
    //     old_size = box_list_.size();
    //     std::cout << "----------QuadTree:begin-----------" << std::endl;
    //     std::cout << "all_box_count: " << box_list_.size() << std::endl;
    //     debug_print(root_, 0);
    //     std::cout << "----------QuadTree:end-----------" << std::endl;
    // }


    // 重构
    timer_ += dt;
    if(timer_ > timer_update_cd_)
    {
        timer_ = 0.f;
        clear(root_);
        for(auto& [box_ptr, count] : box_refcount_)
        {
            _insert(root_, box_ptr);
        }
    }
}

void QuadTree::clear(QuadTreeNode *node)
{
    auto rect = root_->rect_;
    delete root_;
    root_ = new QuadTreeNode(rect);
    // if(!node) return;

    // clear(node->top_left_);
    // clear(node->top_right_);
    // clear(node->bottom_left_);
    // clear(node->bottom_right_);

    // for(auto ptr : node->objects_)
    // {
    //     box_refcount_[ptr]--;
    // }    
    // node->objects_.clear();
    // node->top_left_ = nullptr;
    // node->top_right_ = nullptr;
    // node->bottom_left_ = nullptr;
    // node->bottom_right_ = nullptr;
    // delete node;
}

bool QuadTree::insert(CollideBox* collide_box)
{
    if(box_refcount_.count(collide_box) != 0) return false;
    box_refcount_[collide_box] = 0;
    if(!_insert(root_, collide_box))
    {
        return false;
    }
    return true;
}

bool QuadTree::_insert(QuadTreeNode *node, CollideBox* collide_box)
{
    assert(node != nullptr);
    if(!node->hasIntersection(collide_box)) return false;


    SDL_Log("node_objects-size: %u refcount: %u | insert: %p", node->objects_.size(), 
        box_refcount_[collide_box], collide_box);
    // 不是叶子节点，到叶子节点插入
    if(!node->isLeaf())
    {
        if(_insert(node->top_left_, collide_box)) return true;
        if(_insert(node->top_right_, collide_box)) return true;
        if(_insert(node->bottom_left_, collide_box)) return true;
        if(_insert(node->bottom_right_, collide_box)) return true;
        return false;
    }
    // 当前是叶子,只有叶子存储物体
    else
    {
        node->objects_.emplace_back(collide_box);
        box_refcount_[collide_box]++;
        // 超过最大容量，细分区间，重新分配物体所在区间
        if(node->objects_.size() > max_node_count_ && node->subdevide(grid_min_w_, grid_min_h_))
        {
            // 如果与多个区域重叠，那每个区域都记录
            // Debug:关键，由于物体移动，可以移除了边界，所以每个子区域都可能插入失败:这只能应对不能动的物体
            // v0.1 插入新开辟的子区间（缺点：物体可能移动后一个都不相交，智能指针释放）
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
                SDL_Log("sub insert: %p", old_box);
                if(!_insert(root_, old_box)) SDL_Log("扩容插入失败.");
                if(--box_refcount_[old_box] <= 0)   // 移除原有计数
                {
                    SDL_Log("re insert box failed: %p", old_box);
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
    if(box_refcount_.count(collide_box) == 0) return false;
    _erase(root_, collide_box);
    if(box_refcount_[collide_box] <= 0)
    {
        std::cout << "delete collide box refcount: " << box_refcount_[collide_box] << std::endl;
        delete collide_box;
        box_refcount_.erase(collide_box);
        return true;
    }
    else 
    {
        return false;
    }
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
        // node->objects_.erase(std::remove_if(node->objects_.begin(), node->objects_.end(), 
        // [collide_box](std::shared_ptr<CollideBox>& box)
        // {
        //     return box.get() == collide_box;
        // }), node->objects_.end());

        for(auto iter = node->objects_.begin(); iter != node->objects_.end(); )
        {
            if(*iter == collide_box)
            {
                iter = node->objects_.erase(iter);
                break;
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
        for(auto& src_box : node->objects_)
        {
            if(src_box->getHitLayer() == CollideLayer::None || !src_box->getIsActive()
                || src_box->getCanRemove()) continue;
            for(auto& dst_box : node->objects_)
            {
                if(src_box == dst_box || src_box->getHitLayer() != dst_box->getHurtLayer()
                  || !dst_box->getIsActive() || dst_box->getCanRemove()) continue;

                if(src_box->checkCollision(dst_box))
                {
                    // 记录相撞的碰撞盒子，执行碰撞回调
                    src_box->setOnCollideBox(dst_box);
                    dst_box->setOnCollideBox(src_box);
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
    // if(node->top_left_) _render(node->top_left_);
    // if(node->top_right_) _render(node->top_right_);
    // if(node->bottom_left_) _render(node->bottom_left_);
    // if(node->bottom_right_) _render(node->bottom_right_);
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


// bool QuadTreeNode::isContainCollideBox(const CollideBox *box)
// {
//     assert(!box);
//     bool is_contain = false;
//     if(box->getCollideShape() == CollideShape::Rectangle || box->getCollideShape() == CollideShape::Circle)
//     {
//         auto top_left = box->getPosition() + box->getOffset();
//         auto bottom_right = top_left + box->getSize();
//         if(rect_.x <= top_left.x && rect_.y <= top_left.y 
//             && (rect_.x + rect_.w > bottom_right.x)
//             && (rect_.y + rect_.h > bottom_right.y))
//         {
//             is_contain = true;
//         }
//     }
//     // 其他外形TODO

//     return is_contain;
// }

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

bool QuadTreeNode::subdevide(float grid_min_w, float grid_min_h)
{    
    if(rect_.w <= grid_min_w && rect_.h <= grid_min_h) return false;
    if(!top_left_)
        top_left_ = new QuadTreeNode({rect_.x, rect_.y, rect_.w / 2.f, rect_.h / 2.f});
    if(!top_right_)
        top_right_ = new QuadTreeNode({rect_.x + rect_.w / 2.f, rect_.y, rect_.w / 2.f, rect_.h / 2.f});
    if(!bottom_left_)
        bottom_left_ = new QuadTreeNode({rect_.x, rect_.y + rect_.h / 2.f, rect_.w / 2.f, rect_.h / 2.f});
    if(!bottom_right_)
        bottom_right_ = new QuadTreeNode({rect_.x + rect_.w / 2.f, rect_.y + rect_.h / 2.f, rect_.w / 2.f, rect_.h / 2.f});
    return true;
}
