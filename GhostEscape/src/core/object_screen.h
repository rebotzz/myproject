#ifndef _OBJECT_SCREEN_H_
#define _OBJECT_SCREEN_H_

#include "object.h"
#include "glm/glm.hpp"


// 屏幕对象
class ObjectScreen 
{
protected:
    glm::vec2 render_position_;

public:
    ObjectScreen() {};
    virtual ~ObjectScreen() {};
};



#endif // _OBJECT_SCREEN_H_