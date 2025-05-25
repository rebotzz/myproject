#include "quad_tree.h"
#include "collide_box.h"

QuadTree::~QuadTree()
{
    destroy(root_);
}





bool QuadTree::insert(const CollideBox *collide_box)
{
    return _insert(root_, collide_box);
}

bool QuadTree::_insert(QuadTreeNode *node, const CollideBox *collide_box)
{
    assert(!node);
    if(!node->hasIntersection(collide_box)) return false;

    // 不是叶子节点，到叶子节点插入
    if(!node->isLeaf())
    {
        _insert(node->top_left_, collide_box);
        _insert(node->top_right_, collide_box);
        _insert(node->bottom_left_, collide_box);
        _insert(node->bottom_left_, collide_box);
    }
    // 当前是叶子,只有叶子存储物体
    else
    {
        node->objects_.push_back(const_cast<CollideBox*>(collide_box));
        // 超过最大容量，细分区间，重新分配物体所在区间
        if(node->objects_.size() > max_node_count_ && node->subdevide(grid_min_w_, grid_min_h_))
        {
            // 如果与多个区域重叠，那每个区域都记录
            for(auto& old_box : node->objects_)
            {
                _insert(node->top_left_, old_box);
                _insert(node->top_right_, old_box);
                _insert(node->bottom_left_, old_box);
                _insert(node->bottom_right_, old_box);
            }
            node->objects_.clear();
        }
    }

    return true;
}


bool QuadTree::remove(const CollideBox *collide_box)
{
    
    return _remove(root_, collide_box);
}

bool QuadTree::_remove(QuadTreeNode *node, const CollideBox *collide_box)
{
    if(!node->hasIntersection(collide_box)) return false;

    if(!node->isLeaf())
    {
        _remove(node->top_left_, collide_box);
        _remove(node->top_right_, collide_box);
        _remove(node->bottom_left_, collide_box);
        _remove(node->bottom_right_, collide_box);
    }
    else
    {
        node->objects_.erase(std::remove(node->objects_.begin(), node->objects_.end(), collide_box));
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
        for(auto& src_box : node->objects_)
        {
            for(auto& dst_box : node->objects_)
            {
                if(src_box == dst_box) continue;
                if(src_box->checkCollision(dst_box))
                {
                    src_box->processCollide();
                    dst_box->processCollide();
                }
            }
        }
    }
}


void QuadTree::destroy(QuadTreeNode *node)
{
    if(!node) return;
    if(!node->isLeaf())
    {
        destroy(node->top_left_);
        destroy(node->top_right_);
        destroy(node->bottom_left_);
        destroy(node->bottom_right_);
    }

    delete node;
}


bool QuadTreeNode::isLeaf()
{
    return top_left_ == nullptr && top_right_ == nullptr 
        && bottom_left_ == nullptr && bottom_right_ == nullptr;
}


bool QuadTreeNode::isContainCollideBox(const CollideBox *box)
{
    assert(!box);
    bool is_contain = false;
    if(box->getCollideShape() == CollideShape::Rectangle || box->getCollideShape() == CollideShape::Circle)
    {
        auto top_left = box->getPosition() + box->getOffset();
        auto bottom_right = top_left + box->getSize();
        if(rect_.x <= top_left.x && rect_.y <= top_left.y 
            && (rect_.x + rect_.w > bottom_right.x)
            && (rect_.y + rect_.h > bottom_right.y))
        {
            is_contain = true;
        }
    }
    // 其他外形TODO

    return is_contain;
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

bool QuadTreeNode::subdevide(float grid_min_w, float grid_min_h)
{
    if(rect_.w < grid_min_w && rect_.h < grid_min_h) return false;
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
