#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "object_affiliate.h"

class ObjectScreen;


// 精灵图 (组件)
class Sprite : public ObjectAffiliate
{
protected:
    SDL_Texture* tex_ = nullptr;
    glm::vec2 tex_size_ = glm::vec2(0);
    glm::vec2 render_position_ = glm::vec2(0);

public:
    static Sprite* createAndAddSpriteChild(ObjectScreen* parent, ResID tex_id, float scale = 1.0f, const glm::vec2& offset = glm::vec2(0.0f), AchorMode mode = AchorMode::CENTER);

    virtual void render();

};


#endif // _SPRITE_H_