#include "collide_manager.h"
#include "quad_tree.h"
#include "../core/scene.h"
#include "collide_box.h"

void CollideMgr::insertCollideBox(CollideBox *box)
{
    // 直接插入到四叉树中，由四叉树管理生命周期； 超出边界移除
    quad_tree_->safeInsert(box);
}

void CollideMgr::eraseCollideBox(CollideBox *box)
{
    quad_tree_->erase(box);
}

void CollideMgr::reinit() 
{
    if(quad_tree_)
    {
        delete quad_tree_;
        quad_tree_ = nullptr;
    }
    SDL_FRect init_rect = { 0.f, 0.f, game_.getCurrentScene()->getWorldSize().x, game_.getCurrentScene()->getWorldSize().y};
    quad_tree_ = new QuadTree(init_rect);
    quad_tree_->setCollideMgr(this);
}

void CollideMgr::clean()
{
    // 没必要，场景销毁，世界实体销毁，然后再销毁碰撞盒子。
    // 而场景结束时先调用CollideMgr::clean()，
    // 然后场景析构调用销毁碰撞盒子CollideMgr::eraseCollideBox()
    // 这是bug就出现了：quad_tree_已经是空指针了.
    // if(quad_tree_)
    // {
    //     delete quad_tree_;
    //     quad_tree_ = nullptr;
    // }
}


CollideBox *CollideMgr::createAndInsertCollideBox(Object *parent, CollideShape shape, const glm::vec2 &size, 
    const glm::vec2 &offset, AchorMode achor_mode)
{
    auto collide_box = new CollideBox(parent, shape, size, offset, achor_mode);
    insertCollideBox(collide_box);
    return collide_box;
}

void CollideMgr::notifyCollideBoxIsdeleted(CollideBox *box)
{
    notifyOneObserver(register_wrappers_[box]);
}

CollideMgr::~CollideMgr()
{
    if(quad_tree_) delete quad_tree_;
}

void CollideMgr::update(float dt)
{
    if(!quad_tree_) return;

    timer_collide_ += dt;
    if(timer_collide_ >= time_collide_cd_)
    {
        timer_collide_ = 0.f;
        quad_tree_->update(dt);
    }
}

void CollideMgr::render()
{
#ifdef DEBUG_MODE
    if(!quad_tree_) return;
    quad_tree_->render();
#endif
}   