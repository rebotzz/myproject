#include "collide_manager.h"
#include "quad_tree.h"
#include "../core/scene.h"
#include "collide_box.h"

void CollideMgr::registerCollideBox(CollideBox *box)
{
    if(quad_tree_)
        quad_tree_->safeInsert(box);
}

void CollideMgr::detachCollideBox(CollideBox *box)
{
    if(quad_tree_)
        quad_tree_->erase(box);
}

void CollideMgr::reinit() 
{
    clean();
    SDL_FRect init_rect = { 0.f, 0.f, game_.getCurrentScene()->getWorldSize().x, game_.getCurrentScene()->getWorldSize().y};
    quad_tree_ = new QuadTree(init_rect);
}

void CollideMgr::clean()
{
    // 没必要，场景销毁，世界实体销毁，然后再销毁碰撞盒子。
    // 而场景结束时先调用CollideMgr::clean()，
    // 然后场景析构调用销毁碰撞盒子CollideMgr::eraseCollideBox()
    // 这是bug就出现了：quad_tree_已经是空指针了.

    // 改了一下，如果quad_tree_空了，那么CollideMgr::eraseCollideBox()就什么也不做
    if(quad_tree_)
    {
        delete quad_tree_;
        quad_tree_ = nullptr;
    }
}


CollideMgr::~CollideMgr()
{
    clean();
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
    if(quad_tree_)
        quad_tree_->render();
#endif
}   