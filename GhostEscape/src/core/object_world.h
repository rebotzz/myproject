#ifndef _OBJECT_WORLD_H_
#define _OBJECT_WORLD_H_

#include "object_screen.h"


// 世界对象
class ObjectWorld : public ObjectScreen
{
protected:
    glm::vec2 world_pos_;
    
public:
    ObjectWorld();
    ~ObjectWorld();
};



#endif // _OBJECT_WORLD_H_