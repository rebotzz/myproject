#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "glm/glm.hpp"
#include <vector>

class Scene : public Object
{
protected:
    glm::vec2 camera_position_ = glm::vec2(0.0f);   // 摄像位置
    float camera_zoom_ = 1.0f;                      // 摄像机缩放系数todo
    std::vector<Object*> screen_objects_;           // 区分不同渲染层级，屏幕物体
    std::vector<Object*> world_objects_;            // 世界物体


public:
    Scene() = default;
    virtual ~Scene();

    virtual void init() {};
    virtual void clean() {};
    virtual void handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    virtual void addChild(Object* object) override;
    void cameraFollow(const glm::vec2& target) { camera_position_ = target; }


    // setters and getters
    void setCameraZoom(float camera_zoom) { camera_zoom_ = camera_zoom; }
    const glm::vec2& getCameraPosition() const { return camera_position_; }
    void setCameraPosition(const glm::vec2& position) { camera_position_ = position; }


};

#endif // _SCENE_H_