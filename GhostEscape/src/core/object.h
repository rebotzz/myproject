#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "game.h"
#include "defs.h"

#include <vector>
#include <algorithm>
#include <cassert>

// 所有游戏对象都继承自Object,实体/功能组件
class Object
{
protected:
    Game& game_ = Game::getInstance();
    std::vector<Object*> children_;         // 子节点：存放纯功能组件
    std::vector<Object*> children_to_add_;  // 待添加子节点，防止迭代器失效
    Object* parent_ = nullptr;              // 父节点
    bool is_active_ = true;                 // 是否启用
    ObjectType type_ = ObjectType::None;    // 类型
    bool can_remove_ = false;               // 是否需要移除,(标记后下次遍历前移除，避免delete this)

public:
    Object() = default;
    Object(Object* parent, ObjectType type = ObjectType::None);
    virtual ~Object();
    virtual bool handleEvent(const SDL_Event& event);
    virtual void update(float dt);
    virtual void render();

    // 工具函数
    virtual void addChild(Object* object) { children_.push_back(object); }
    virtual void safeAddChild(Object* object) { children_to_add_.push_back(object); };  // 避免update时更新vector，迭代器失效
    virtual void removeChild(Object* object) 
    { children_.erase(std::remove(children_.begin(), children_.end(), object), children_.end()); }

    // setters and getters
    void setParent(Object* parent) { parent_ = parent; }
    Object* getParent() const { return parent_; }
    void setActive(bool flag) { is_active_ = flag; }
    bool getIsActive() const { return is_active_; }
    void setObjectType(ObjectType type) { type_ = type; }
    ObjectType getObjectType() const { return type_; }
    bool getCanRemove() const { return can_remove_; }
    void setCanRemove(bool flag) { can_remove_ = flag; }

protected:
    void removeInvalidObject();
};

#endif // _OBJECT_H_