#include "quad_tree.h"
#include "collide_box.h"
#include "../core/scene.h"
#include <memory>


QuadTreeNode::QuadTreeNode(const SDL_FRect &rect)
    :in_rect_(rect)
{
    // 出口边界采用入口边界两倍
    out_rect_ = {in_rect_.x - in_rect_.w / 2.f, in_rect_.y - in_rect_.h / 2.f, in_rect_.w * 2.f, in_rect_.h * 2.f};
}

bool QuadTreeNode::isLeaf()
{
    for(auto subnode : next_)
    {
        if(subnode != nullptr) return false;
    }
    return true;
}

bool QuadTreeNode::hasInRectIntersection(const CollideBox *box)
{
    return hasIntersection(in_rect_, box);
}

bool QuadTreeNode::hasOutRectIntersection(const CollideBox *box)
{
    return hasIntersection(out_rect_, box);
}

bool QuadTreeNode::hasIntersection(const SDL_FRect &rect, const CollideBox *box)
{
    bool intersect = false;
    if(CollideShape::Rectangle == box->getCollideShape())
    {
        auto top_left = box->getPosition() + box->getOffset();
        auto size = box->getScaledSize();
        SDL_FRect rect_box = {top_left.x, top_left.y, size.x, size.y};
        if(SDL_HasRectIntersectionFloat(&rect_box, &rect))
            intersect = true;
    }
    else if(CollideShape::Circle == box->getCollideShape())
    {
        if(CollideBox::hasIntersectionRectAndCircle(glm::vec2(rect.x, rect.y), {rect.w, rect.h}, 
        box->getPosition(), box->getScaledSize().x))
        {
            intersect = true;
        }
    }

    return intersect;
}

bool QuadTreeNode::split(float min_grid_w, float min_grid_h)
{    
    if(in_rect_.w <= min_grid_w && in_rect_.h <= min_grid_h) return false;

    float sub_w = in_rect_.w / 2.f, sub_h = in_rect_.h / 2.f;
    for(int i = 0; i < sizeof(next_) / sizeof(QuadTreeNode*); ++i)
    {
        if(nullptr == next_[i])
        {
            SDL_FRect rect = {in_rect_.x + static_cast<float>(i % 2) * sub_w, in_rect_.y + static_cast<float>(i / 2) * sub_h, sub_w, sub_h};
            next_[i] = new QuadTreeNode(rect);
        }
    }
    return true;
}


QuadTree::~QuadTree()
{
    destroyTreeNode(root_);
    colliders_refcount_.clear();
    colliders_to_insert_.clear();
}

void QuadTree::destroyTreeNode(QuadTreeNode *&node)
{
    if(!node) return;
    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        destroyTreeNode(node->next_[i]);
    }

    // 到了叶子节点：清理区域碰撞盒子，删除节点，置空Node指针
    for(auto ptr : node->colliders_)
    {
        // debug:不能直接用[],要检测是否存在,不然会先插入
        if(colliders_refcount_.count(ptr))
            --colliders_refcount_[ptr];
    }    
    node->colliders_.clear();
    delete node;
    node = nullptr;
}


void QuadTree::clear()
{
    auto old_rect = root_->in_rect_;
    destroyTreeNode(root_);
    root_ = new QuadTreeNode(old_rect);
}


void QuadTree::update(float dt)
{
    // 加入待添加碰撞盒子
    if(!colliders_to_insert_.empty())
    {
        for(auto& box_ptr : colliders_to_insert_) insert(box_ptr);
        colliders_to_insert_.clear();
    }
    // 碰撞盒子位置更新
    for(auto [collider, count] : colliders_refcount_)
    {
        if(collider->getIsActive())
            collider->update(dt);
    }
    // 更新物体所在区域v1.0: 重构四叉树
    clear();
    for(auto& [box_ptr, count] : colliders_refcount_)
    {
        _insert(root_, box_ptr);
    }
    // // 更新物体所在区域v2.0：边界检测
    // updateCollidersRegion(root_);
    // reblanceTree(root_);
    // for(auto& [box_ptr, count] : colliders_refcount_) // 尝试插入之前更新过程中插入失败的。
    // {
    //     if(count <= 0) _insert(root_, box_ptr);
    // }

    // 处理碰撞
    // checkAndProcessCollide();
    checkAndProcessCollide2();
    // debugPrint(root_, 0, "root", true);
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
    if(!node->hasInRectIntersection(collide_box)) return false;

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
            return true;

        node->colliders_.emplace_back(collide_box);
        if(colliders_refcount_.count(collide_box)) ++colliders_refcount_[collide_box];
        // else SDL_Log("[Error] _insert ++, but colliders_refcount_ not exists!, ptr: %p", collide_box);
        
        // 超过最大容量，细分区间，重新分配物体所在区间,当前节点变为索引，不存储
        if(node->colliders_.size() > max_node_count_ && node->split(min_grid_size_.x, min_grid_size_.y))
        {
            // 如果与多个区域重叠，那每个区域都记录
            // v0.2 从根节点找合适的位置插入    此时非叶子节点，不会插入node
            for(auto& old_box : node->colliders_)
            {
                if(colliders_refcount_.count(old_box)) --colliders_refcount_[collide_box];
                // else SDL_Log("[Error] _insert --, but colliders_refcount_ not exists!, ptr: %p", collide_box);
                if(!_insert(root_, old_box))
                {
                    // TODO： 一般来说不会插入失败，除非物体脱离了世界边界。子弹？或者碰撞盒子初始位置在世界之外？ 
                    // 或许可以重试几次，如果失败再处理。直接移除碰撞盒子那么会导致以后都不能发送碰撞，即使物体之后可以插入
                    // 在每一次更新时检测：当refcount == 0时，尝试插入
                    // SDL_Log("re insert failed: %p", old_box);   
                }
            }
            node->colliders_.clear();
        }
        return true;
    }
}

bool QuadTree::erase(CollideBox* collide_box)
{
    // 如果正确，这里erase后引用计数应该变为0
    _erase(root_, collide_box);     
    // if(!colliders_refcount_.count(collide_box))
    //     SDL_Log("[Error], erase box failed, ptr not exists in colliders_refcount_! ptr: %p\n", collide_box);
    // if(colliders_refcount_.count(collide_box) && colliders_refcount_[collide_box] != 0)
    //     SDL_Log("[Error], after _erase box, ptr: %p, refcount: %d\n", collide_box, colliders_refcount_[collide_box]);
    colliders_refcount_.erase(collide_box);
    colliders_to_insert_.erase(std::remove(colliders_to_insert_.begin(), colliders_to_insert_.end(), collide_box), colliders_to_insert_.end());
    return true;
}

bool QuadTree::_erase(QuadTreeNode *node, CollideBox* collide_box)
{
    if(!node->hasInRectIntersection(collide_box)) return false;

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
                if(colliders_refcount_.count(box)) --colliders_refcount_[box];
                // else SDL_Log("[Error] _erase box not find in colliders_refcount_, %p", collide_box);
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

void QuadTree::checkAndProcessCollide2()
{
    std::unordered_set<CollideBox*> visited;    // 标记已经访问过的
    // 遍历管理的碰撞盒子，检索与它再统一区域的其他碰撞盒，执行碰撞
    for(auto [collider, count] : colliders_refcount_)
    {
        if(count <= 0 || !collider->getIsActive() || collider->getCanRemove()) continue;
        std::unordered_set<CollideBox*> same_rect_colliders;
        retrieve(root_, collider, same_rect_colliders);
        if(same_rect_colliders.empty()) continue;
        for(auto other_collider : same_rect_colliders)
        {
            if(visited.count(other_collider) || !other_collider->getIsActive() || other_collider->getCanRemove()) continue;
            if(collider->checkCollision(other_collider))
            {
                if((collider->getHitLayer() != CollideLayer::None && collider->getHitLayer() == other_collider->getHurtLayer())
                || (collider->getHurtLayer() != CollideLayer::None && collider->getHurtLayer() == other_collider->getHitLayer()))
                {
                    collider->processCollide(other_collider);
                    other_collider->processCollide(collider);
                }
            }
        }
        visited.insert(collider);
    }
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
                // long long hash_key = reinterpret_cast<long long>(src_box) + reinterpret_cast<long long>(dst_box);
                long long hash_key = std::hash<CollideBox*>{}(src_box) ^ (std::hash<CollideBox*>{}(dst_box) << 1);
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
    // 渲染自身区域网格，如果子区域存在，递归渲染. 这里世界坐标需要转化为渲染坐标
    auto pos_in_rect = Game::getInstance().getCurrentScene()->worldToScreen({node->in_rect_.x, node->in_rect_.y});
    SDL_FRect grid_render_in_rect = {pos_in_rect.x, pos_in_rect.y, node->in_rect_.w, node->in_rect_.h};
    Game::getInstance().renderRect(grid_render_in_rect, {1.0f, 0.0f, 0.0f, 1.f});
    // auto pos_out_rect = Game::getInstance().getCurrentScene()->worldToScreen({node->out_rect_.x, node->out_rect_.y});
    // SDL_FRect grid_render_out_rect = {pos_out_rect.x, pos_out_rect.y, node->out_rect_.w, node->out_rect_.h};
    // Game::getInstance().renderRect(grid_render_out_rect, {0.0f, 0.5f, 0.0f, 1.f});

    // 渲染处于统一区域的碰撞盒子. 鼠标在的区域碰撞盒子改为蓝色颜色
    SDL_FColor box_color = {1.f,1.f,1.f,1.f};
    glm::vec2 mouse_render_pos;
    Game::getInstance().getMouseState(mouse_render_pos);
    auto mouse_world_pos = Game::getInstance().getCurrentScene()->screenToWorld(mouse_render_pos);
    SDL_FPoint mouse_point = {mouse_world_pos.x, mouse_world_pos.y};
    if(SDL_PointInRectFloat(&mouse_point, &node->in_rect_)) box_color = {0.f,1.f,1.f,1.f};
    for(auto box : node->colliders_)
    {
        auto pos_box = Game::getInstance().getCurrentScene()->worldToScreen(box->getPosition());
        SDL_FRect box_rect = {pos_box.x, pos_box.y, box->getScaledSize().x, box->getScaledSize().y};
        Game::getInstance().renderRect(box_rect, box_color);
    }
    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        _render(node->next_[i]);
    }
}




void QuadTree::retrieve(QuadTreeNode* node, CollideBox* box, std::unordered_set<CollideBox*>& result) 
{
    if(nullptr == node || nullptr == box || !node->hasInRectIntersection(box)) return;
    if(node->isLeaf()) 
    {
        for(auto collider : node->colliders_) 
            if(collider != box) result.insert(collider);
    } 
    else 
    {
        for(auto child : node->next_) retrieve(child, box, result);
    }
}

void QuadTree::debugPrint(QuadTreeNode* node, int deep, const std::string& desc, bool print_first)
{
    if(!node) return;

    if(print_first)
    {
        SDL_Log("\n\n----------------quad_tree----------------begin:\n");
        SDL_Log("box_refcount:\n");
        for (auto iter : colliders_refcount_)
        {
            SDL_Log("[ptr: %p, cnt: %d]", iter.first, iter.second);
        }
        debugPrint(root_, 0, "root");
        SDL_Log("----------------quad_tree----------------end!\n\n");
        return;
    }

    SDL_Log("deep: %d desc: %s colliders_box_count: %u\n", deep, desc.c_str(), node->colliders_.size());
    if(node->colliders_.size())
    {
        SDL_Log("box_list: ");
        for(auto ptr : node->colliders_) SDL_Log("%p\t", ptr);
        SDL_Log("\n");
    }

    for(int i = 0; i < sizeof(node->next_) / sizeof(QuadTreeNode*); ++i)
    {
        debugPrint(node->next_[i], deep + 1, "region_" + std::to_string(i));
    }
}


void QuadTree::updateCollidersRegion(QuadTreeNode *node)
{
    if(!node) return;

    if(!node->isLeaf())
    {
        // 不是子节点，递归更新. 这里不适合合并分支平衡树，因为有插入操作
        for(auto child_node : node->next_) updateCollidersRegion(child_node);
    }
    else if(!node->colliders_.empty())
    {
        // 离开了出口边界,从当前区域移除，插入到新的区域
        for(auto iter = node->colliders_.begin(); iter != node->colliders_.end(); )
        {
            CollideBox* collider = *iter;
            if(!node->hasOutRectIntersection(collider)) 
            {
                iter = node->colliders_.erase(iter);
                if(colliders_refcount_.count(collider)) --colliders_refcount_[collider];
                // else SDL_Log("[Error] updateCollidersRegion, but box not find in colliders_refcount_, ptr: %p", collider);
                if(!_insert(root_, collider))     // TODO
                {
                    // SDL_Log("re insert failed: %p", collider);
                }
            }
            else
            {
                ++iter;
            }
        }
    }
}

size_t QuadTree::reblanceTree(QuadTreeNode *node)
{
    if(!node) return 0;
    if(node->isLeaf()) return node->colliders_.size();

    size_t child_colliders_count = 0;  // 以当前节点为根节点，其所有子节点包含碰撞盒子数量
    for(auto child_node : node->next_) 
    {
        child_colliders_count += reblanceTree(child_node);
    }

    // 数量少于阈值，释放空间合并节点
    if(child_colliders_count < max_node_count_)
    {
        // debug:不同子区间可能有相同碰撞盒子(横跨多个区间)
        std::unordered_map<CollideBox*, int> box_count;
        for(auto& child_node : node->next_) 
        {
            if(nullptr == child_node) continue;
            for(auto box : child_node->colliders_) ++box_count[box];
            child_node->colliders_.clear();
            delete child_node;
            child_node = nullptr;
        }
        for(auto [box, count] : box_count)
        {
            node->colliders_.push_back(box);
            if(count > 1) 
            {
                if(colliders_refcount_.count(box)) colliders_refcount_[box] -= (count - 1);
                // else SDL_Log("[Error] reblanceTree, but box not find in colliders_refcount_, ptr: %p", box);
            }
        }
    }
    return child_colliders_count;
}
