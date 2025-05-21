#include "collide_box.h"
#include "object_world.h"


CollideBox *CollideBox::createAndAddCollideBoxChild(Object *parent, CollideShape shape, const glm::vec2 &size, 
    const glm::vec2 &offset, AchorMode achor_mode)
{
    auto collide_box = new CollideBox();
    collide_box->setCollideShape(shape);
    collide_box->setAchorModeAndSize(achor_mode, size);
    collide_box->setRelativeOffset(offset);
    if(parent)
    {
        parent->safeAddChild(collide_box);
        collide_box->setParent(parent);
    }
    return collide_box;
}

void CollideBox::update(float)
{
    // 更新位置: 渲染坐标？ 还是世界坐标？ 如果所有都偏移了，那么相交判断可以进行; 结论：都行
    position_ = dynamic_cast<ObjectScreen*>(parent_)->getRenderPosition() + offset_;
}

void CollideBox::render()
{
#ifdef DEBUG_MODE
    SDL_FRect rect = {position_.x, position_.y, size_.x, size_.y};
    static SDL_Texture* tex_circle = game_.getAssetStore().getTexture(ResID::Tex_Circle);
    SDL_SetTextureAlphaModFloat(tex_circle, 0.5f);
    game_.renderTexture(ResID::Tex_Circle, nullptr, &rect);

#endif
}


bool CollideBox::checkCollision(CollideBox *target)
{
    if(shape_ == CollideShape::Circle && target->getCollideShape() == CollideShape::Circle)
    {
        float distance = glm::length(position_ - target->getPosition());
        if(distance <= (size_.x / 2 + target->getSize().x / 2)) 
            return true;
    }
    else if(shape_ == CollideShape::Rectangle && target->getCollideShape() == CollideShape::Rectangle)
    {
        SDL_FRect rect_1 = { position_.x, position_.y, position_.x + size_.x, position_.y + size_.y };
        SDL_FRect rect_2 = { target->getPosition().x,  target->getPosition().y, 
             target->getPosition().x + target->getSize().x,  target->getPosition().y + target->getSize().y };
        if(SDL_HasRectIntersectionFloat(&rect_1, &rect_2)) 
            return true;
    }
    // todo: 圆与矩形相交

    return false;
}