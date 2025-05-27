#include "collide_box.h"
#include "../core/object_world.h"
#include "collide_manager.h"
#include "../core/scene.h"

CollideBox::CollideBox(Object *parent, CollideShape shape, const glm::vec2 &size, const glm::vec2 &offset, 
    AchorMode achor_mode)
    :ObjectAffiliate(parent)
{
    setCollideShape(shape);
    setAchorModeAndSize(achor_mode, size);
    setRelativeOffset(offset);

    SDL_Log("CollideBox(): %p", this);

    game_.getCurrentScene()->getCollideMgr()->registerCollideBox(this);

}

CollideBox::~CollideBox()
{
    SDL_Log("~CollideBox(): %p", this);
    game_.getCurrentScene()->getCollideMgr()->detachCollideBox(this);
}


void CollideBox::update(float)
{
    // 更新位置， 碰撞检测：渲染坐标/世界坐标都行，都偏移了相交判断可以进行
    auto render_pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    position_ = game_.getCurrentScene()->screenToWorld(render_pos);
}

void CollideBox::render()
{
#ifdef DEBUG_MODE
    auto render_pos = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
    SDL_FRect rect = {render_pos.x, render_pos.y, size_.x, size_.y};
    if(CollideShape::Circle == shape_)
    {
        static SDL_Texture* tex_circle = game_.getAssetStore().getTexture(ResID::Tex_Circle);
        SDL_SetTextureAlphaModFloat(tex_circle, 0.5f);
        game_.renderTexture(ResID::Tex_Circle, nullptr, &rect);
    }
    else if(CollideShape::Rectangle == shape_)
    {
        game_.renderFillRect(render_pos, size_, {0.f, 1.f, 0.f, 0.4f});
    }
#endif
}


bool CollideBox::checkCollision(CollideBox *target)
{
    bool is_collide = false;
    if(shape_ == CollideShape::Circle && target->getCollideShape() == CollideShape::Circle)
    {
        float distance = glm::length(position_ - target->getPosition());
        if(distance <= (size_.x / 2 + target->getSize().x / 2)) 
            is_collide = true;
    }
    else if(shape_ == CollideShape::Rectangle && target->getCollideShape() == CollideShape::Rectangle)
    {
        SDL_FRect rect_1 = { position_.x, position_.y, position_.x + size_.x, position_.y + size_.y };
        SDL_FRect rect_2 = { target->getPosition().x,  target->getPosition().y, 
             target->getPosition().x + target->getSize().x,  target->getPosition().y + target->getSize().y };
        if(SDL_HasRectIntersectionFloat(&rect_1, &rect_2)) 
            is_collide = true;
    }
    else 
    {
        auto rect_pos = glm::vec2(0), circle_pos = glm::vec2(0);
        auto rect_size = glm::vec2(0);
        float circle_size = 0.f;
        if(CollideShape::Rectangle == shape_)
        {
            rect_pos = position_ + offset_;
            rect_size = size_;
            circle_pos = target->getPosition();
            circle_size = target->getSize().x;
        }
        else
        {
            rect_pos = target->getPosition() + target->getOffset();
            rect_size = target->getSize();
            circle_pos = position_;
            circle_size = size_.x;
        }
        if(checkIntersectRectCircle(rect_pos, rect_size, circle_pos, circle_size))
            is_collide = true;
    }

    return is_collide;
}

// 判断圆与矩形相交
bool CollideBox::checkIntersectRectCircle(const glm::vec2 &rect_position, const glm::vec2 &rect_size, 
    const glm::vec2 &circle_position, float circle_size)
{
    // 方法1：基于圆心到矩形最短距离的算法；方法2：依据矩形边划分为9个区域，分类讨论
    auto rect_center = rect_position + rect_size / 2.0f;
    auto delta = glm::abs(circle_position - rect_center);
    auto min_distance = delta - rect_size / 2.0f;
    if(min_distance.x < 0) min_distance.x = 0.f;
    if(min_distance.y < 0) min_distance.y = 0.f;
    return glm::dot(min_distance, min_distance) <= circle_size * circle_size;
}




