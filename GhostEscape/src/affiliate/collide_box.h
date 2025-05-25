#ifndef _COLLIDE_BOX_H_
#define _COLLIDE_BOX_H_

#include "object_affiliate.h"
#include <functional>

// v1.0碰撞盒子作为组件由主体直接管理内存和生命周期
// 特点：直接使用简单方便，但引入碰撞管理器和空间四叉树对于生命周期控制不方便
// 移除碰撞盒子需要通知持有方（碰撞主体、碰撞管理器），需要观察者模式
// v2.0由碰撞管理器处理所有碰撞盒子生命周期,

class CollideBox : public ObjectAffiliate
{
private:
    CollideShape shape_ = CollideShape::Circle; // 碰撞外形    
    glm::vec2 position_ = glm::vec2(0); // 如果碰撞外形为圆时,size_.x = size_.y = 直径
    std::function<void()> on_collide_;   // 碰撞发生时处理逻辑
    CollideBox *intersection_box_ = nullptr;
    CollideLayer hit_layer = CollideLayer::None;
    CollideLayer hurt_layer = CollideLayer::None;

private:
    CollideBox() = default;
    ~CollideBox() = default;

public:
    static CollideBox* createAndAddCollideBoxChild(Object* parent, CollideShape shape, const glm::vec2& size, 
        const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);

    virtual void update(float) override;
    virtual void render() override;
    
    bool checkCollision(CollideBox* target);
    void processCollide() { if(on_collide_) on_collide_(); }

    static bool checkIntersectRectCircle(const glm::vec2& rect_position, const glm::vec2& rect_size, 
        const glm::vec2& circle_position, float circle_size);

    // setters and getters
    void setCollideShape(CollideShape shape) { shape_ = shape; }
    CollideShape getCollideShape() const { return shape_; }
    void setPosition(const glm::vec2& position) { position_ = position; }
    const glm::vec2& getPosition() const { return position_; }
    void setOnCollideCallback(const std::function<void()>& callback) { on_collide_ = callback; }
    CollideBox* getOnCollideBox() const { return intersection_box_; }
    void setOnCollideBox(CollideBox* box) { intersection_box_ = box; }
    void setHitLayer(CollideLayer type) { hit_layer = type; }
    CollideLayer getHitLayer() const { return hit_layer; }
    void setHurtLayer(CollideLayer type) { hurt_layer = type; }
    CollideLayer getHurtLayer() const { return hurt_layer; }
};





#endif // _COLLIDE_BOX_H_