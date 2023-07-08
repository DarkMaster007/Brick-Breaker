#pragma once
#ifndef CPADDLE_H
#define CPADDLE_H
#endif // CPADDLE_H

#include "defines.h"

class cPaddle {
  private:
    float x, y;
    float speed;
    int originalX, originalY;
    int paddle_width, paddle_height;
    int bounceReverseArea;           // How big the area is where the ball goes back instead of forward
  public:
    static Texture2D textureBody;
    static Texture2D textureEdge_L;
    static Texture2D textureEdge_R;

    cPaddle(int posX, int posY, int width, int height);
    inline int getX();
    inline int getY();
    inline Vector2 getSize();
    inline float getSpeed();
    inline int getBounceReverseArea();
    void moveLeft();
    void moveLeft(int);
    void moveRight();
    void moveRight(int);
    void Reset();
    void Logic();
    static void Draw(cPaddle *paddle);
    void Input();
};

int cPaddle::getX() {
    return x;
}
int cPaddle::getY() {
    return y;
}
Vector2 cPaddle::getSize() {
    return {(float)paddle_width, (float)paddle_height};
}
float cPaddle::getSpeed() {
    return speed;
}
int cPaddle::getBounceReverseArea() {
    return bounceReverseArea;
}
