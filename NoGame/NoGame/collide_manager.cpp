#include "collide_manager.h"
#include "quad_tree.h"
#include "collide_box.h"
#include "../core/scene.h"

void CollideMgr::registerCollideBox(CollideBox *box)
{
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    if(quad_tree_)
        quad_tree_->safeInsert(box);
#else
    colliders_to_insert_.push_back(box);
#endif  // COLLIDE_MGR_VERSION_QUADTREE_
}

void CollideMgr::detachCollideBox(CollideBox *box)
{
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    if(quad_tree_)
        quad_tree_->erase(box);
#else
    colliders_.erase(std::remove(colliders_.begin(), colliders_.end(), box), colliders_.end());
#endif  // COLLIDE_MGR_VERSION_QUADTREE_
}

void CollideMgr::reinit() 
{
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    clean();
    SDL_FRect init_rect = { 0.f, 0.f, game_.getCurrentScene()->getWorldSize().x, game_.getCurrentScene()->getWorldSize().y};
    quad_tree_ = new QuadTree(init_rect);
#else
    colliders_.clear();
#endif  // COLLIDE_MGR_VERSION_QUADTREE_
}

void CollideMgr::clean()
{
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    // debug: 场景结束会clean,场景删除也会clean。改了一下，别的接口加入quad_tree_判空。
    if(quad_tree_)
    {
        delete quad_tree_;
        quad_tree_ = nullptr;
    }
#else
    colliders_.clear();
#endif  // COLLIDE_MGR_VERSION_QUADTREE_

}

void CollideMgr::checkAndProcessCollide2For()
{
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
#else
    for(auto src_box : colliders_)
    {
        if(src_box->getHitLayer() == CollideLayer::None || !src_box->getIsActive()
            || src_box->getCanRemove()) continue;
        for(auto dst_box : colliders_) // A->B,B->A
        {
            if(src_box == dst_box || src_box->getHitLayer() != dst_box->getHurtLayer()
                || !dst_box->getIsActive() || dst_box->getCanRemove()) continue;

            if(src_box->checkCollision(dst_box))
            {
                src_box->processCollide(dst_box);
                dst_box->processCollide(src_box);
            }
        }
    }
#endif  // COLLIDE_MGR_VERSION_QUADTREE_
}

CollideMgr::~CollideMgr()
{
    clean();
}

void CollideMgr::update(float dt)
{
    timer_collide_ += dt;
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    if(!quad_tree_) return;
    if(timer_collide_ >= time_collide_cd_)
    {
        timer_collide_ = 0.f;
        quad_tree_->update(dt);
    }
#else
    if(!colliders_to_insert_.empty()) 
    {
        colliders_.insert(colliders_.end(), colliders_to_insert_.begin(), colliders_to_insert_.end());
        colliders_to_insert_.clear();
    }
    checkAndProcessCollide2For();
#endif  // COLLIDE_MGR_VERSION_QUADTREE_

}

void CollideMgr::render()
{
#ifdef DEBUG_MODE
#ifdef COLLIDE_MGR_VERSION_QUADTREE_
    if(quad_tree_)
        quad_tree_->render();
#endif  // COLLIDE_MGR_VERSION_QUADTREE_
#endif  // DEBUG_MODE

}   