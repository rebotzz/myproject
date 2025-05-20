#ifndef _OBJECT_AFFILIATE_H_
#define _OBJECT_AFFILIATE_H_

#include "object.h"
#include "defs.h"

class ObjectAffiliate : public Object
{
protected:
    glm::vec2 size_ = glm::vec2(0);     // 尺寸
    glm::vec2 offset_ = glm::vec2(0);   // 偏移
    AchorMode achor_mode_ = AchorMode::CENTER;  // 锚点模式

public:
    ObjectAffiliate() = default;
    virtual ~ObjectAffiliate() {};

    // setters and getters
    void setSize(const glm::vec2& size) { size_ = size; }
    const glm::vec2& getSize() const { return size_; }
    void setOffset(const glm::vec2& offset) { offset_ = offset; }
    const glm::vec2& getOffset() const { return offset_; }
    void setAchorMode(AchorMode mode);
    AchorMode getAchorMode() const { return achor_mode_; }
    void setAchorModeAndSize(AchorMode mode, const glm::vec2& size);
    void setRelativeOffset(const glm::vec2& relative_offset) { offset_ += relative_offset; }  // 现有偏移上累加
};



#endif // _OBJECT_AFFILIATE_H_