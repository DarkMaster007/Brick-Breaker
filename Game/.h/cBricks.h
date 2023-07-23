#pragma once
#ifndef CBRICKS_H
#define CBRICKS_H
#endif // CBRICKS_H

#include "defines.h"
#include "cBall.h"

class cPowerup;

class cBricks {
  public:
    static Texture2D texture;
    static int brickCount;
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    bool enabled;

    cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType);
    Color getColor();
    void Reset();
    static void Logic(cBricks *brick, cBall *ball, cPowerup *powerup, Sound soundBounceGeneral);
    static void Draw(cBricks *brick);
    static void Input();
};
