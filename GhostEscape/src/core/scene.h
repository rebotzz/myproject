#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "glm/glm.hpp"
#include <vector>

class Scene : public Object
{
protected:
    glm::vec2 world_size_ = glm::vec2(0);           // 世界大小
    glm::vec2 camera_position_ = glm::vec2(0);      // 摄像位置
    float camera_zoom_ = 1.0f;                      // 摄像机缩放系数todo
    std::vector<Object*> screen_objects_;           // 区分不同渲染层级，屏幕物体
    std::vector<Object*> world_objects_;            // 世界物体
    bool pause_time_ = false;                       // 时间暂停

    // 其他：有纵深的星空背景
    int stars_count = 2000;
    std::vector<glm::vec2> bg_stars, mid_stars, fg_stars;  // 星星坐标
    bool init_stars = false;

public:
    Scene() = default;
    virtual ~Scene();

    virtual void init() {};
    virtual void clean() {};
    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    virtual void addChild(Object* object) override;
    void cameraFollow(const glm::vec2& target) { camera_position_ = target; }
    glm::vec2 worldToScreen(const glm::vec2& world_position) { return world_position - camera_position_; }
    glm::vec2 screenToWorld(const glm::vec2& screen_position) { return screen_position + camera_position_; }

    // setters and getters
    void setCameraZoom(float camera_zoom);
    const glm::vec2& getCameraPosition() const { return camera_position_; }
    void setCameraPosition(const glm::vec2& position);
    const glm::vec2& getWorldSize() const { return world_size_; }
    void setWorldSize(const glm::vec2& size) { world_size_ = size; }
    std::vector<Object*>& getWorldObjects() { return world_objects_; }
    std::vector<Object*>& getScreenObjects() { return screen_objects_; }
    void setPauseTime(bool flag) { pause_time_ = flag; }
    bool getIsPauseTime() const { return pause_time_; }

protected:
    void removeInvalidObject(); // 隐藏父类接口，这里似乎没有必要用虚函数
    virtual void renderStarsBackGround();
};

#endif // _SCENE_H_