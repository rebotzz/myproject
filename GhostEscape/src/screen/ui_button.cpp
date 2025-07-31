#include "ui_button.h"

UIButton::UIButton(Object *parent, const glm::vec2 &position, const glm::vec2 &scale, 
    ResID tex_idle, ResID tex_hover, ResID tex_clicked)
{
    if(parent)
    {
        parent->safeAddChild(this);
        setParent(parent);
    }
    setObjectType(ObjectType::Screen);
    setRenderPosition(position);

    sprite_idle_ = new Sprite(this, tex_idle, AchorMode::CENTER, scale);
    sprite_hover_ = new Sprite(this, tex_hover, AchorMode::CENTER, scale);
    sprite_clicked_ = new Sprite(this, tex_clicked, AchorMode::CENTER, scale);
    current_sprite_ = sprite_idle_;
    sprite_hover_->setActive(false);
    sprite_clicked_->setActive(false);
}

UIButton *UIButton::createAndAddUIButtonChild(Object *parent, const glm::vec2 &position, const glm::vec2 &scale, ResID tex_idle, ResID tex_hover, ResID tex_clicked)
{
    auto ui_button = new UIButton(parent, position, scale, tex_idle, tex_hover, tex_clicked);
    return ui_button;
}

bool UIButton::handleEvent(const SDL_Event& event)
{ 
    if(ObjectScreen::handleEvent(event)) return true;

    bool is_on_button = game_.isMouseInRect(sprite_idle_->getLeftTopPosition(), sprite_idle_->getLeftTopPosition() + sprite_idle_->getSize());
    glm::vec2 mouse_pos;
    bool is_mouse_left_down = game_.getMouseState(mouse_pos) & SDL_BUTTON_LMASK;
    switch(event.type)
    {
        case SDL_EVENT_MOUSE_MOTION:
        switch(button_status_)
        {
            case ButtonStatus::IDLE: if(is_on_button && !is_mouse_left_down) button_status_ = ButtonStatus::HOVER; break;
            case ButtonStatus::HOVER: if(!is_on_button) button_status_ = ButtonStatus::IDLE; break;
            case ButtonStatus::CLICKED: if(is_on_button && !is_mouse_left_down) button_status_ = ButtonStatus::IDLE; break;
        }
        break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if(event.button.button == SDL_BUTTON_LEFT && button_status_ == ButtonStatus::HOVER)
        {
            button_status_ = ButtonStatus::CLICKED;
            game_.playSound(ResID::Sound_UIButton08);
            return true;
        }
        break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
        if(button_status_ == ButtonStatus::CLICKED && is_on_button)
        {
            if(on_clicked_) on_clicked_();
            return true;
        }
        break;
    }

    return false;
}

void UIButton::update(float dt)
{
    ObjectScreen::update(dt);
    updateButton();
}


void UIButton::updateButton()
{
    // 更新按键图片
    auto old_sprite = current_sprite_;
    switch(button_status_)
    {
        case ButtonStatus::IDLE: current_sprite_ = sprite_idle_; break;
        case ButtonStatus::HOVER: current_sprite_ = sprite_hover_; break;
        case ButtonStatus::CLICKED: current_sprite_ = sprite_clicked_; break;
    }
    if(old_sprite != current_sprite_)
    {
        old_sprite->setActive(false);
        current_sprite_->setActive(true);
        // TODO
        // current_sprite_->setRenderPosition(old_sprite->getRenderPosition());

        if(current_sprite_ != sprite_clicked_ && button_status_ != ButtonStatus::CLICKED)
            game_.playSound(ResID::Sound_UIButton12);
    }
}


void UIButton::setSize(const glm::vec2& size)
{
    sprite_idle_->setSize(size);
    sprite_hover_->setSize(size);
    sprite_clicked_->setSize(size);
}
