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
            std::cout << ptr.get() << "\t";
        }
        std::cout << std::endl;
    }

    debug_print(node->top_left_, deep + 1);
    debug_print(node->top_right_, deep + 1);
    debug_print(node->bottom_left_, deep + 1);
    debug_print(node->bottom_right_, deep + 1);
}

void QuadTree::update(float )
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
    static size_t old_size = box_list_.size();
    if(old_size != box_list_.size())
    {
        old_size = box_list_.size();
        std::cout << "----------QuadTree:begin-----------" << std::endl;
        std::cout << "all_box_count: " << box_list_.size() << std::endl;
        debug_print(root_, 0);
        std::cout << "----------QuadTree:end-----------" << std::endl;
    }

}


bool QuadTree::insert(CollideBox* collide_box)
{
    // SDL_Log("QuadTree::insert -------------\nrect[x,y,w,h]:\t"
    //     "quad_tree_rect[%f,%f,%f,%f], collide_box_rect:[%f,%f,%f,%f]",
    //     root_->rect_.x, root_->rect_.y, root_->rect_.w, root_->rect_.h,
    // collide_box->getPosition().x, collide_box->getPosition().y,
    // collide_box->getSize().x, collide_box->getSize().y);
    // SDL_Log("QuadTree::insert-std::shared_ptr<CollideBox> [%p] count: %d", collide_box.get(), collide_box.use_count());
    auto ptr = std::shared_ptr<CollideBox>(collide_box);
    box_list_.push_back(ptr);
    if(!_insert(root_, ptr))
    {
        return false;
    }
    // SDL_Log("QuadTree::insert-std::shared_ptr<CollideBox> [%p] count: %d", collide_box.get(), collide_box.use_count());
    return true;
}

bool QuadTree::_insert(QuadTreeNode *node, const std::shared_ptr<CollideBox>& collide_box)
{
    assert(node != nullptr);
    if(!node->hasIntersection(collide_box.get())) return false;
    // SDL_Log("_insert()-begin: std::shared_ptr<CollideBox> [%p] count: %d", collide_box.get(), collide_box.use_count());

    // 不是叶子节点，到叶子节点插入
    if(!node->isLeaf())
    {
        _insert(node->top_left_, collide_box);
        _insert(node->top_right_, collide_box);
        _insert(node->bottom_left_, collide_box);
        _insert(node->bottom_right_, collide_box);
    }
    // 当前是叶子,只有叶子存储物体
    else
    {
        node->objects_.emplace_back(collide_box);
        // SDL_Log("\n递达叶子节点,插入 ptr: %p, size: %u\n", collide_box.get(), node->objects_.size());
        // 超过最大容量，细分区间，重新分配物体所在区间
        if(node->objects_.size() > max_node_count_ && node->subdevide(grid_min_w_, grid_min_h_))
        {
            SDL_Log("\n超过最大容量\n");

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
                if(!_insert(root_, old_box)) SDL_Log("扩容插入失败.");
                old_box = nullptr;
            }
            node->objects_.clear();

        }
    }

    return true;
}


bool QuadTree::erase(CollideBox* collide_box)
{
    // 这里不能用智能指针，因为shared_ptr只管理拷贝于自生的，新建的智能指针是一个单独的个体，
    // 与原来的没有关联，虽然指向同一对象地址：这会导致delete两次报错。
    for(auto iter = box_list_.begin(); iter != box_list_.end(); )
    {
        if(iter->get() == collide_box)
        {
            iter = box_list_.erase(iter);
        }
        else 
        {
            ++iter;
        }
    }
    return _erase(root_, collide_box);
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

        for(auto iter = node->objects_.begin(); iter != node->objects_.end(); ++iter)
        {
            if(iter->get() == collide_box)
            {
                iter = node->objects_.erase(iter);
                break;
            }
        }
    }

    return true;
}


void QuadTree::processCollide()
{
    _processCollide(root_);
}

void QuadTree::_processCollide(QuadTreeNode *node)
{
    if(!node) return;

    if(!node->isLeaf())
    {
        _processCollide(node->top_left_);
        _processCollide(node->top_right_);
        _processCollide(node->bottom_left_);
        _processCollide(node->bottom_right_);
    }
    else 
    {
        // 非叶子节点不存储对象，所以到叶子节点检测碰撞
        for(auto& src_box : node->objects_)
        {
            if(src_box->getHitLayer() == CollideLayer::None) continue;
            for(auto& dst_box : node->objects_)
            {
                if(src_box == dst_box || src_box->getHitLayer() != dst_box->getHurtLayer()) continue;

                if(src_box->checkCollision(dst_box.get()))
                {
                    // 记录相撞的碰撞盒子，执行碰撞回调
                    src_box->setOnCollideBox(dst_box.get());
                    dst_box->setOnCollideBox(src_box.get());
                    src_box->processCollide();
                    dst_box->processCollide();
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
    if(rect_.w < grid_min_w && rect_.h < grid_min_h) return false;
    SDL_Log("\n区间细分 count: %u rect:[%f,%f,%f,%f]\n", objects_.size(),
        rect_.x, rect_.y, rect_.w, rect_.h);

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
