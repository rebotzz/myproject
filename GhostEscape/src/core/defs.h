#ifndef _DEFS_H_
#define _DEFS_H_

#define DEBUG_MODE

enum class AchorMode
{
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

enum class ObjectType
{
    None, 
    Screen,
    World,
    Enemy,      
    Player,
};

enum class CollideLayer
{
    None,
    Enemy,      
    Player,
};

enum class CollideShape
{
    Circle,
    Rectangle
};

#endif // _DEFS_H_