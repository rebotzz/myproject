#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "game.h"
#include "defs.h"

#include <vector>

// 所有游戏对象都继承自Object,实体/功能组件
class Object
{
protected:
    Game& game_ = Game::getInstance();
    std::vector<Object*> children_;     // 子节点：组件
    std::vector<Object*> children_to_add_;     
    Object* parent_ = nullptr;          // 父节点
    bool is_active_ = true;             // 是否启用
    ObjectType type_ = ObjectType::None;

public:
    Object() = default;
    virtual ~Object();
    virtual void handleEvent(const SDL_Event& event);
    virtual void update(float dt);
    virtual void render();

    // 工具函数
    virtual void addChild(Object* object) { children_.push_back(object); }
    virtual void safeAddChild(Object* object) { children_to_add_.push_back(object); };  // 避免update时更新vector，迭代器失效

    // setters and getters
    void setParent(Object* parent) { parent_ = parent; }
    Object* getParent() const { return parent_; }
    void setActive(bool flag) { is_active_ = flag; }
    bool getIsActive() const { return is_active_; }
    void setObjectType(ObjectType type) { type_ = type; }
    ObjectType getObjectType() const { return type_; }
};

#endif // _OBJECT_H_