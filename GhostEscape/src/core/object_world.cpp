#include "object_world.h"
#include "scene.h"

void ObjectWorld::update(float dt)
{
    ObjectScreen::update(dt);

    // // 更新渲染坐标；世界物体必须挂载到场景，不然没有意义。
    syncRenderPosition();

    // if(!parent_) return;
    // auto scene = dynamic_cast<Scene*>(parent_);
    // // 不能超出世界边界
    // world_position_ = glm::clamp(world_position_, glm::vec2(0), scene->getWorldSize());
    // updateRenderPosition(dynamic_cast<Scene*>(parent_)->getCameraPosition());
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