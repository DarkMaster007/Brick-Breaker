#pragma once
#ifndef CPADDLE_H
#define CPADDLE_H
#endif // CPADDLE_H

#include "defines.h"

class cPaddle {
private:
    int x, y;
    int originalX, originalY;
    int paddle_width, paddle_height;
    int bounceReverseArea;           // How big the area is where the ball goes back instead of forward
public:
    cPaddle(int posX, int posY, int width, int height);
    inline int getX();
    inline int getY();
    inline void moveLeft(int pixels);
    inline void moveRight(int pixels);
    inline Vector2 getSize();
    inline int getBounceReverseArea();
    void Reset();
};

int cPaddle::getX(){return x;}
int cPaddle::getY(){return y;}
void cPaddle::moveLeft(int pixels){x -= pixels;}
void cPaddle::moveRight(int pixels){x += pixels;}
Vector2 cPaddle::getSize(){return {(float)paddle_width, (float)paddle_height};}
int cPaddle::getBounceReverseArea(){return bounceReverseArea;}
