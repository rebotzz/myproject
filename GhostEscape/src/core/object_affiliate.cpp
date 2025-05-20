#include "object_affiliate.h"
#include "object_screen.h"

void ObjectAffiliate::setAchorMode(AchorMode mode)
{
    // 可能在锚点模式下还有偏移量，这里偏移累加
    switch(achor_mode_)
    {
        case AchorMode::TOP_LEFT: offset_ += glm::vec2(0.0f); break;
        case AchorMode::TOP_CENTER: offset_ += glm::vec2(-size_.x / 2, 0.0f); break;
        case AchorMode::TOP_RIGHT: offset_ += glm::vec2(- size_.x, 0.0f); break;
        case AchorMode::CENTER_LEFT: offset_ += glm::vec2(0.0f, - size_.y / 2); break;
        case AchorMode::CENTER: offset_ += glm::vec2(- size_.x / 2, - size_.y / 2); break;
        case AchorMode::CENTER_RIGHT: offset_ += glm::vec2(- size_.x, - size_.y / 2); break;
        case AchorMode::BOTTOM_LEFT: offset_ += glm::vec2(0.0f, - size_.y); break;
        case AchorMode::BOTTOM_CENTER: offset_ += glm::vec2(- size_.x / 2, - size_.y); break;
        case AchorMode::BOTTOM_RIGHT: offset_ += glm::vec2(- size_.x, - size_.y); break;
    }
}

void ObjectAffiliate::setAchorModeAndSize(AchorMode mode, const glm::vec2 &size)
{
    size_ = size;
    achor_mode_ = mode;
    setAchorMode(achor_mode_);
}
