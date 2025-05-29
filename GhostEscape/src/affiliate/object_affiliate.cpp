#include "object_affiliate.h"
#include "../core/object_screen.h"

void ObjectAffiliate::setSize(const glm::vec2 &size)
{
    size_ = size;
    setOffsetByAchor();
}

void ObjectAffiliate::setOffsetByAchor()
{
    // 可能在锚点模式下还有偏移量，这里就不累加偏移，如果需要就使用setRelativeOffset
    // 需要清楚：偏移是依据自身size,自身的某个位置处于父节点位置.
    switch(achor_mode_)
    {
        case AchorMode::NONE: offset_ = glm::vec2(0); break;
        case AchorMode::TOP_LEFT: offset_ = glm::vec2(0.0f); break;
        case AchorMode::TOP_CENTER: offset_ = glm::vec2(-size_.x / 2, 0.0f); break;
        case AchorMode::TOP_RIGHT: offset_ = glm::vec2(- size_.x, 0.0f); break;
        case AchorMode::CENTER_LEFT: offset_ = glm::vec2(0.0f, - size_.y / 2); break;
        case AchorMode::CENTER: offset_ = glm::vec2(- size_.x / 2, - size_.y / 2); break;
        case AchorMode::CENTER_RIGHT: offset_ = glm::vec2(- size_.x, - size_.y / 2); break;
        case AchorMode::BOTTOM_LEFT: offset_ = glm::vec2(0.0f, - size_.y); break;
        case AchorMode::BOTTOM_CENTER: offset_ = glm::vec2(- size_.x / 2, - size_.y); break;
        case AchorMode::BOTTOM_RIGHT: offset_ = glm::vec2(- size_.x, - size_.y); break;
    }
}

void ObjectAffiliate::setAchorMode(AchorMode mode)
{
    achor_mode_ = mode;
    setOffsetByAchor();
}

void ObjectAffiliate::setAchorModeAndSize(AchorMode mode, const glm::vec2 &size)
{
    size_ = size;
    achor_mode_ = mode;
    setOffsetByAchor();
}

void ObjectAffiliate::setScale(const glm::vec2 &scale)
{
    scale_ = scale;
    size_ *= scale;
    setOffsetByAchor();
}
