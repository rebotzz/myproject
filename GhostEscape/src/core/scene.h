#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "glm/glm.hpp"
#include <vector>


class CollideMgr;
// 场景
class Scene : public Object
{
protected:
    glm::vec2 world_size_ = glm::vec2(0);           // 世界大小
    glm::vec2 camera_position_ = glm::vec2(0);      // 摄像位置
    std::vector<Object*> screen_children_;           // 区分不同渲染层级，屏幕物体
    std::vector<Object*> world_children_;            // 世界物体
    bool pause_time_ = false;                       // 时间暂停

    // 碰撞管理器
    CollideMgr* collide_mgr_ = nullptr;

    // 其他：有纵深的星空背景
    int stars_count = 2000;
    std::vector<glm::vec2> bg_stars, mid_stars, fg_stars;  // 星星坐标
    bool init_stars = false;

    // TODO:摄像机缩放系数，SDL的设置逻辑分辨率窗口缩放也有类似效果，但是不够方便；位置、尺寸都要修改。
    // 或许需要单独一个摄像机类，由摄像机类绘制，计算缩放后的位置、尺寸。
    float camera_zoom_ = 1.0f;

public:
    Scene();
    virtual ~Scene();

    virtual void init() {};
    virtual void clean() {};
    virtual bool handleEvent(const SDL_Event& event) override;
    virtual void update(float dt) override;
    virtual void render() override;

    // 工具函数
    virtual void addChild(Object* object) override;
    virtual void removeChild(Object* object) override;
    void cameraFollow(const glm::vec2& target) { camera_position_ = target; }
    void cameraFollow(float dt, const glm::vec2& target);
    glm::vec2 worldToScreen(const glm::vec2& world_position) { return world_position - camera_position_; }
    glm::vec2 screenToWorld(const glm::vec2& screen_position) { return screen_position + camera_position_; }
    bool checkBeyoundScreen(const glm::vec2& world_position, const glm::vec2& size, const glm::vec2& margin = glm::vec2(30));

    // setters and getters
    void setCameraZoom(float camera_zoom);
    const glm::vec2& getCameraPosition() const { return camera_position_; }
    void setCameraPosition(const glm::vec2& position);
    const glm::vec2& getWorldSize() const { return world_size_; }
    void setWorldSize(const glm::vec2& size) { world_size_ = size; }
    std::vector<Object*>& getWorldObjects() { return world_children_; }
    std::vector<Object*>& getScreenObjects() { return screen_children_; }
    void setPauseTime(bool flag) { pause_time_ = flag; }
    bool getIsPauseTime() const { return pause_time_; }
    CollideMgr* getCollideMgr() const { return collide_mgr_; }

protected:
    void updateCamera(float dt);
    void removeInvalidObject(); // 隐藏父类接口，这里似乎没有必要用虚函数
    virtual void renderStarsBackGround();
};

#endif // _SCENE_H_