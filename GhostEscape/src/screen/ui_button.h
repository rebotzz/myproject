#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "../core/object_screen.h"
#include "../affiliate/sprite.h"
#include <functional>

// 按键类   屏幕对象，挂载到场景
class UIButton : public ObjectScreen
{
    enum class ButtonStatus
    {
        IDLE, HOVER, CLICKED 
    };
protected:
    ButtonStatus button_status_ = ButtonStatus::IDLE;
    Sprite* sprite_idle_ = nullptr;
    Sprite* sprite_hover_ = nullptr;
    Sprite* sprite_clicked_ = nullptr;
    Sprite* current_sprite_ = nullptr;
    std::function<void()> on_clicked_;  // 按键点击执行逻辑

public:
    UIButton(Object* parent, const glm::vec2& position, const glm::vec2 &scale, 
        ResID tex_idle, ResID tex_hover, ResID tex_clicked);
    static UIButton* createAndAddUIButtonChild(Object* parent, const glm::vec2& position, const glm::vec2 &scale, 
        ResID tex_idle, ResID tex_hover, ResID tex_clicked);
    
    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    
    // setters andd getters
    void setOnClickCallback(const std::function<void()>& callback) { on_clicked_ = callback; }
    void setSize(const glm::vec2& size);
    glm::vec2 getSize() { return sprite_idle_->getSize(); }

protected:
    void updateButton();
};

#endif // _UI_BUTTON_H_