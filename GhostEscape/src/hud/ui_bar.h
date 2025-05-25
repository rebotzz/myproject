#ifndef _UI_BAR_H_
#define _UI_BAR_H_

#include "../affiliate/object_affiliate.h"

class Actor;

class UIBar : public ObjectAffiliate
{
protected:
    float percentage_ = 1.0f;          // 百分比
    
public:
    UIBar() { setObjectType(ObjectType::Screen); }  // ??? 但是设置这个没用，它属于角色。不归场景
    ~UIBar() = default;

    virtual void render();

    // setters and getters
    void setPercentage(float val);
};


#endif // _UI_BAR_H_