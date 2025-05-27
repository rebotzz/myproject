#ifndef _COLLIDE_BOX_H_
#define _COLLIDE_BOX_H_

#include "object_affiliate.h"
#include "../core/object.h"
#include "../core/observer.h"
#include <functional>

// v1.0碰撞盒子作为组件由主体直接管理内存和生命周期
// 特点：直接使用简单方便，但引入碰撞管理器和空间四叉树对于生命周期控制不方便
// 移除碰撞盒子需要通知持有方（碰撞主体、碰撞管理器），需要观察者模式
// v2.0由碰撞管理器处理所有碰撞盒子生命周期,  
// 特点：改用一个包装器包装碰撞盒子，当自动挂载delete包装器时，析构函数给碰撞管理器发送信号，
// 然后实际删除碰撞盒子。避免了多个观察者.但是更加复杂更容易写bug


class CollideBox : public ObjectAffiliate
{
    friend class CollideBoxWrapper;
    friend class QuadTree;
    friend class QuadTreeNode;
    friend class CollideMgr;
private:
    CollideShape shape_ = CollideShape::Circle; // 碰撞外形    
    glm::vec2 position_ = glm::vec2(0); // 如果碰撞外形为圆时,size_.x = size_.y = 直径
    std::function<void(CollideBox *)> on_collide_;   // 碰撞发生时处理逻辑
    CollideBox *intersection_box_ = nullptr;
    CollideLayer hit_layer = CollideLayer::None;
    CollideLayer hurt_layer = CollideLayer::None;

private:
    CollideBox() = default;
    CollideBox(Object *parent, CollideShape shape, const glm::vec2 &size,
                const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);

public:
    ~CollideBox()
    {
#ifdef DEBUG_MODE
        SDL_Log("~CollideBox(): %p", this);
#endif
    }

    virtual void update(float) override;
    virtual void render() override;
    
    // 工具函数
    bool checkCollision(CollideBox* target);
    void processCollide(CollideBox * target) { if(on_collide_) on_collide_(target); }
    static bool checkIntersectRectCircle(const glm::vec2& rect_position, const glm::vec2& rect_size, 
        const glm::vec2& circle_position, float circle_size);

    // setters and getters
    void setCollideShape(CollideShape shape) { shape_ = shape; }
    CollideShape getCollideShape() const { return shape_; }
    void setPosition(const glm::vec2& position) { position_ = position; }
    const glm::vec2& getPosition() const { return position_; }
    void setOnCollideCallback(const std::function<void(CollideBox *)>& callback) { on_collide_ = callback; }
    CollideBox* getOnCollideBox() const { return intersection_box_; }
    void setOnCollideBox(CollideBox* box) { intersection_box_ = box; }
    void setHitLayer(CollideLayer type) { hit_layer = type; }
    CollideLayer getHitLayer() const { return hit_layer; }
    void setHurtLayer(CollideLayer type) { hurt_layer = type; }
    CollideLayer getHurtLayer() const { return hurt_layer; }
};


// 碰撞盒包装器，只是做一个中转，辅助碰撞管理器管理碰撞盒子生命周期；
// 因为直接改原有代码太麻烦了，所以加入一个中间层
class CollideBoxWrapper : public Object, public Observer
{
private:
    CollideBox* collide_box_ = nullptr;

public:
    CollideBoxWrapper(Object* parent, CollideShape shape, const glm::vec2& size, 
        const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);
    ~CollideBoxWrapper();
    static CollideBoxWrapper* createAndAddCollideBoxChild(Object* parent, CollideShape shape, const glm::vec2& size, 
        const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);

    virtual void update(float dt) override { collide_box_->update(dt); };
    virtual void render() override { collide_box_->render(); };
    
    bool checkCollision(CollideBoxWrapper* target) { return collide_box_->checkCollision(target->collide_box_); };
    void processCollide(CollideBox * target) { if(collide_box_->on_collide_) collide_box_->on_collide_(target); }

    // setters and getters
    void setCollideShape(CollideShape shape) { collide_box_->setCollideShape(shape); }
    CollideShape getCollideShape() const { return collide_box_->getCollideShape(); }
    void setPosition(const glm::vec2& position) { collide_box_->setPosition(position); }
    const glm::vec2& getPosition() const { return collide_box_->getPosition(); }
    void setOnCollideCallback(const std::function<void(CollideBox *)>& callback) { collide_box_->setOnCollideCallback(callback); }
    CollideBox* getOnCollideBox() const { return collide_box_->getOnCollideBox(); }
    void setOnCollideBox(CollideBox* box) { collide_box_->setOnCollideBox(box); }
    void setHitLayer(CollideLayer type) { collide_box_->setHitLayer(type); }
    CollideLayer getHitLayer() const { return collide_box_->getHitLayer(); }
    void setHurtLayer(CollideLayer type) { collide_box_->setHurtLayer(type); }
    CollideLayer getHurtLayer() const { return collide_box_->getHurtLayer(); }

    // 临时debug
    CollideBox* getInternalCollideBox() const { return collide_box_; }

    // 基类隐藏
    Object* getParent() const { return collide_box_->getParent(); }
    void setSize(const glm::vec2& size) { collide_box_->setSize(size); }
    const glm::vec2& getSize() const { return collide_box_->getSize(); }
};





#endif // _COLLIDE_BOX_H_