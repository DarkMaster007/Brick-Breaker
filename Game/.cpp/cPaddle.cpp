#include "cPaddle.h"

cPaddle::cPaddle(int posX, int posY, int width, int height)
{
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    paddle_width = width;
    paddle_height = height;
    bounceReverseArea = 13;
    speed = 15;
}

void cPaddle::Reset()
{
    x = originalX;
    y = originalY;
}

void cPaddle::moveLeft()
{
    x -= speed + GetFrameTime();
}

void cPaddle::moveLeft(int a)
{
    x -= a;
}

void cPaddle::moveRight()
{
    x += speed + GetFrameTime();
}

void cPaddle::moveRight(int a)
{
    x += a;
}
