#include "cPaddle.h"

int cPaddle::instanceCount = 0;

cPaddle::cPaddle(float posX, float posY, int width, int height)
{
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    paddle_width = width;
    paddle_height = height;
    bounceReverseArea = 8;
    instanceCount++;
}
int cPaddle::getX()
{
    return x;
}
int cPaddle::getY()
{
    return y;
}
void cPaddle::setX(float newX){
    x = newX;
}
void cPaddle::Reset()
{
    x = originalX;
    y = originalY;
}
void cPaddle::Move()
{
    float frametime = GetFrameTime();
    x += GetMouseDelta().x - speed * IsKeyDown(KEY_A) * frametime + speed * IsKeyDown(KEY_D) * frametime;
}
Vector2 cPaddle::getSize()
{
    return {(float)paddle_width, (float)paddle_height};
}
int cPaddle::getBounceReverseArea()
{
    return bounceReverseArea;
}
void cPaddle::setSpeed(float newSpeed){
    speed = newSpeed;
}

cPaddle::~cPaddle(){
    instanceCount--;
}
