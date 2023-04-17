#pragma once
#include "defines.h"

class cPaddle {
private:
    float x, y;
    float originalX, originalY;
    int paddle_width, paddle_height;
    int bounceReverseArea;           // How big the area is where the ball goes back instead of forward
    float speed{900};
public:
    static int instanceCount;

    cPaddle(float posX, float posY, int width, int height);
    int getX();
    int getY();
    void setX(float);
    void Reset();
    void Move();
    Vector2 getSize();
    int getBounceReverseArea();
    void setSpeed(float newSpeed);
    ~cPaddle();
};
