#pragma once
#include "defines.h"

class cBricks
{
public:
    static int instanceCount;
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    bool enabled;
    cBricks();
    ~cBricks();
};
