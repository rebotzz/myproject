#ifndef _OBJECT_AFFILIATE_H_
#define _OBJECT_AFFILIATE_H_

#include "../core/object.h"
#include "../core/defs.h"

class ObjectAffiliate : public Object
{
protected:
    glm::vec2 size_ = glm::vec2(0);     // 尺寸
    glm::vec2 offset_ = glm::vec2(0);   // 偏移
    AchorMode achor_mode_ = AchorMode::CENTER;  // 锚点模式
    glm::vec2 scale_ = glm::vec2(1);    // 缩放

public:
    ObjectAffiliate() = default;
     ObjectAffiliate(Object* parent):Object(parent) {}
    virtual ~ObjectAffiliate() {};

    // setters and getters
    virtual void setSize(const glm::vec2& size);
    const glm::vec2& getSize() const { return size_; }
    void setOffset(const glm::vec2& offset) { offset_ = offset; }   // 设置绝对偏移
    void setRelativeOffset(const glm::vec2& relative_offset) { offset_ += relative_offset; }  // 设置相对偏移，现有偏移上累加
    void setOffsetByAchor();                                        // 依据锚点模式设置偏移
    const glm::vec2& getOffset() const { return offset_; }
    void setAchorMode(AchorMode mode);
    AchorMode getAchorMode() const { return achor_mode_; }
    void setAchorModeAndSize(AchorMode mode, const glm::vec2& size);
    const glm::vec2& getScale() const { return scale_; }
    void setScale(const glm::vec2& scale);
};



#endif // _OBJECT_AFFILIATE_H_