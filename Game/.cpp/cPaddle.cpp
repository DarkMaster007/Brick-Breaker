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
}

void cPaddle::Reset(){x = originalX;y = originalY;}
