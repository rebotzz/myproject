#ifndef _OBJECT_AFFILIATE_H_
#define _OBJECT_AFFILIATE_H_

#include "../core/object.h"
#include "../core/defs.h"

// 重构，ObjectAffiliate舍弃scale，只要size就行，scale属于继承类的拓展
class ObjectAffiliate : public Object
{
protected:
    glm::vec2 offset_ = glm::vec2(0);   // 偏移
    AchorMode achor_mode_ = AchorMode::CENTER;  // 锚点模式
    glm::vec2 size_ = glm::vec2(0);     // 尺寸, 因为锚点、缩放、偏移相互影响，所以私有化size，要获取就通过getScaledSize()

public:
    ObjectAffiliate() = default;
    ObjectAffiliate(Object* parent, AchorMode mode = AchorMode::CENTER);
    virtual ~ObjectAffiliate() {};

    // setters and getters
    // debug: achor_mode、size、scale是关联在一起的
    virtual void setSize(const glm::vec2& size);
    glm::vec2 getSize() const { return size_; }
    void setOffset(const glm::vec2& offset) { offset_ = offset; }   // 设置绝对偏移
    void setRelativeOffset(const glm::vec2& relative_offset) { offset_ += relative_offset; }  // 设置相对偏移，现有偏移上累加
    void setOffsetByAchor();                                        // 依据锚点模式设置偏移
    const glm::vec2& getOffset() const { return offset_; }
    void setAchorMode(AchorMode mode);
    AchorMode getAchorMode() const { return achor_mode_; }
    void setAchorModeAndSize(AchorMode mode, const glm::vec2& size);
    void setScale(const glm::vec2& scale);
};



#endif // _OBJECT_AFFILIATE_H_