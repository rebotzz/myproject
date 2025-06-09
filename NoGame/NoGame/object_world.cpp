#include "object_world.h"
#include "scene.h"

void ObjectWorld::update(float dt)
{
    ObjectScreen::update(dt);

    // syncRenderPosition();
}

const glm::vec2& ObjectWorld::getRenderPosition()
{
    render_position_ = game_.getCurrentScene()->worldToScreen(world_position_); // 更新渲染坐标
    return render_position_;
}

void ObjectWorld::setRenderPosition(const glm::vec2& position)
{
    ObjectScreen::setRenderPosition(position);
    world_position_ = game_.getCurrentScene()->screenToWorld(position);
}
void ObjectWorld::setPosition(const glm::vec2& position)
{
    world_position_ = position;
    render_position_ = game_.getCurrentScene()->worldToScreen(position);
}