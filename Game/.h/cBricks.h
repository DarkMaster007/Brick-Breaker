#pragma once
#ifndef CBRICKS_H
#define CBRICKS_H
#endif // CBRICKS_H

struct Bricks {
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    bool enabled;
};
