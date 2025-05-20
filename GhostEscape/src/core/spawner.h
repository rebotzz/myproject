#ifndef _SPAWNER_H_
#define _SPAWNER_H_


#include "object_world.h"
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
            auto object = dynamic_cast<ObjectWorld*>(new T());
            object->setPosition(pos);
            object->setParent(parent);
            parent->safeAddChild(object);
        }
    }
};




#endif // _SPAWNER_H_