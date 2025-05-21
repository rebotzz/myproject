#ifndef _COLLIDE_BOX_H_
#define _COLLIDE_BOX_H_

#include "object_affiliate.h"

class CollideBox : public ObjectAffiliate
{
private:
    CollideShape shape_ = CollideShape::Circle; // 碰撞外形    
    glm::vec2 position_ = glm::vec2(0);
    // 如果碰撞外形为圆时,size_.x = size_.y = 直径

public:
    CollideBox() = default;
    ~CollideBox() = default;
    static CollideBox* createAndAddCollideBoxChild(Object* parent, CollideShape shape, const glm::vec2& size, 
        const glm::vec2& offset = glm::vec2(0), AchorMode achor_mode = AchorMode::CENTER);

    virtual void update(float) override;
    virtual void render() override;
    
    bool checkCollision(CollideBox* target);

    // setters and getters
    void setCollideShape(CollideShape shape) { shape_ = shape; }
    CollideShape getCollideShape() const { return shape_; }
    void setPosition(const glm::vec2& position) { position_ = position; }
    const glm::vec2& getPosition() const { return position_; }
};





#endif // _COLLIDE_BOX_H_