#ifndef _TEXT_LABLE_
#define _TEXT_LABLE_

#include "../affiliate/object_affiliate.h"

// 文本标签
class TextLable : public ObjectAffiliate
{
protected:
    TTF_Text* ttf_text_ = nullptr;
    SDL_FColor text_color_ = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec2 render_center_positon_ = glm::vec2(0);

public:
    TextLable(Object* parent, const std::string& text, ResID font_id, int font_size, 
        SDL_FColor color = {1.0f, 1.0f, 1.0f, 1.0f}, AchorMode mode = AchorMode::CENTER);
    ~TextLable();

    virtual void render() override;

    void setText(const std::string& text);
    void setTextColor(SDL_FColor text_color) { text_color_ = text_color; }
    void setRenderCenterPosition(const glm::vec2& position) { render_center_positon_ = position; }
};

#endif // _TEXT_LABLE_