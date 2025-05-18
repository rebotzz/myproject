#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "glm/glm.hpp"


class Scene : public Object
{
protected:
    glm::vec2 camera_position_ = glm::vec2(0.0f);   // 摄像位置
    float camera_zoom_ = 1.0f;      // 摄像机缩放系数

public:
    Scene() = default;
    virtual ~Scene() {};

    virtual void init() {};
    virtual void clean() {};
};

#endif // _SCENE_H_