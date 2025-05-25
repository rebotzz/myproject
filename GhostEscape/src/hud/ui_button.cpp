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

    sprite_idle_ = Sprite::createAndAddSpriteChild(this, tex_idle, scale);
    sprite_hover_ = Sprite::createAndAddSpriteChild(this, tex_hover, scale);
    sprite_clicked_ = Sprite::createAndAddSpriteChild(this, tex_clicked, scale);
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

    switch(event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_UP:
        glm::vec2 mouse_pos;
        game_.getMouseState(mouse_pos);
        SDL_FPoint cursor_point = {mouse_pos.x, mouse_pos.y};
        SDL_FRect button_rect = { sprite_idle_->getRenderPosition().x,
            sprite_idle_->getRenderPosition().y,
            sprite_idle_->getSize().x,
            sprite_idle_->getSize().y};
        if(button_status_ == ButtonStatus::CLICKED && SDL_PointInRectFloat(&cursor_point, &button_rect))
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
    auto button_pos = sprite_idle_->getRenderPosition();
    auto button_size = sprite_idle_->getSize();
    SDL_FRect button_rect = {button_pos.x, button_pos.y, button_size.x, button_size.y};
    glm::vec2 cursor_pos;
    auto mouse_button_state = game_.getMouseState(cursor_pos);
    SDL_FPoint cursor_point = {cursor_pos.x, cursor_pos.y};
    bool is_on_button = SDL_PointInRectFloat(&cursor_point, &button_rect); 
    // 按键状态切换
    switch(button_status_)
    {
        case ButtonStatus::IDLE:
        if(is_on_button && !mouse_button_state) button_status_ = ButtonStatus::HOVER;
        break;
        case ButtonStatus::HOVER:
        if(!is_on_button) button_status_ = ButtonStatus::IDLE;
        else if(mouse_button_state & SDL_BUTTON_LMASK) 
        {
            button_status_ = ButtonStatus::CLICKED;
            game_.playSound(ResID::Sound_UIButton08);
        }
        break;
        case ButtonStatus::CLICKED:
        if(!is_on_button) button_status_ = ButtonStatus::IDLE;
        // 按钮点击执行的逻辑放在了handleEvent()
        break;
    }

    // 跟新按键图片
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
        current_sprite_->setRenderPosition(old_sprite->getRenderPosition());

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
