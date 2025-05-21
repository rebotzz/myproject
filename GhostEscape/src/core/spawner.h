#ifndef _SPAWNER_H_
#define _SPAWNER_H_


#include "object_world.h"
#include "scene.h"
#include <vector>

// 生成器
template<class T>
class Spawner : public Object
{
private:
    int count_ = 0;
    glm::vec2 start_position_ = glm::vec2(0);
    glm::vec2 end_position_ = glm::vec2(0);
public:
    Spawner(int count, const glm::vec2& start_position, const glm::vec2& end_position)
        :count_(count), start_position_(start_position), end_position_(end_position)
    {}
    ~Spawner() {};

    void spawnAndAddChild(Object* parent)
    {
        for(int i = 0; i < count_; ++i)
        {
            auto pos = game_.getRandomVec2(start_position_, end_position_);
            new T(parent, pos);     // 构造函数完成了节点挂载,虽然看起来像是内存泄漏
            // // 更新渲染坐标，避免生成后闪现,不，不应该是这样的，毕竟safeAddChild延时添加，在渲染之前一定跟新的渲染坐标
            // object->updateRenderPosition(dynamic_cast<Scene*>(parent)->getCameraPosition());
        }
    }
};




#endif // _SPAWNER_H_