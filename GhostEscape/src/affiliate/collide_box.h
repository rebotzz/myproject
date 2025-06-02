#ifndef _COLLIDE_BOX_H_
#define _COLLIDE_BOX_H_

#include "object_affiliate.h"
#include <functional>


// 主体持有碰撞盒子，并且管理碰撞盒子生命周期。
// 而碰撞管理器只是持有指针，当碰撞主体销毁（连带它的组件-碰撞盒子）时，
// 给碰撞管理器发送消息，删除指针。构造：注册，析构：取消注册。

// 碰撞盒子(组件)
class CollideBox : public ObjectAffiliate
{
private:
    CollideShape shape_ = CollideShape::Circle;         // 碰撞外形。 如果碰撞外形为圆时,size_.x = size_.y = 直径
    glm::vec2 position_ = glm::vec2(0);                 // 位置。
    std::function<void(CollideBox *)> on_collide_;      // 碰撞发生时处理逻辑
    CollideLayer hit_layer = CollideLayer::None;
    CollideLayer hurt_layer = CollideLayer::None;

public:
    CollideBox(Object *parent, CollideShape shape, const glm::vec2 &size,
                const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);
    ~CollideBox();

    virtual void update(float) override;
    virtual void render() override;
    
    // 工具函数
    bool checkCollision(CollideBox* target);
    void processCollide(CollideBox * target) { if(on_collide_) on_collide_(target); }
    static bool hasIntersectionRectAndCircle(const glm::vec2& rect_position, const glm::vec2& rect_size, 
        const glm::vec2& circle_position, float circle_size);   // 判断圆与矩形相交

    // setters and getters
    void setCollideShape(CollideShape shape) { shape_ = shape; }
    CollideShape getCollideShape() const { return shape_; }
    void setPosition(const glm::vec2& position) { position_ = position; }
    const glm::vec2& getPosition() const { return position_; }
    void setOnCollideCallback(const std::function<void(CollideBox *)>& callback) { on_collide_ = callback; }
    void setHitLayer(CollideLayer type) { hit_layer = type; }
    CollideLayer getHitLayer() const { return hit_layer; }
    void setHurtLayer(CollideLayer type) { hurt_layer = type; }
    CollideLayer getHurtLayer() const { return hurt_layer; }
};

#endif // _COLLIDE_BOX_H_