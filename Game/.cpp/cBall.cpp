#include "cBall.h"

int cBall::instanceCount = 0;

cBall::cBall(int posX, int posY, int ball_size)
{
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    direction = STOP;
    originalSize = ball_size;
    current_size = ball_size;
    randomizeMovement();
    instanceCount++;
}
void cBall::Reset()
{
    x = originalX;
    y = originalY;
    direction = STOP;
    current_size = originalSize;
}
void cBall::changeDirection(eDir d)
{
    direction = d;
}
void cBall::randomDirection()
{
    direction = (eDir)(randomNrDirection(marsenneTwister));
}
int cBall::getX()
{
    return x;
}
int cBall::getY()
{
    return y;
}
eDir cBall::getDirection()
{
    return direction;
}
void cBall::changeSize(int ball_size)
{
    current_size = ball_size;
}
int cBall::getSize()
{
    return current_size;
}
void cBall::setSpeed(float newSpeed){
    speed = newSpeed;
}
void cBall::randomizeMovement()
{
    randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
    randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
}
void cBall::Move()
{
    switch (direction)
    {
    case STOP:
        break;
    case UPLEFT:
        x-= (speed + randomMovementOffset[0]) * GetFrameTime();
        y-= (speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case DOWNLEFT:
        x-= (speed + randomMovementOffset[0]) * GetFrameTime();
        y+= (speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case UPRIGHT:
        x+= (speed + randomMovementOffset[0]) * GetFrameTime();
        y-= (speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case DOWNRIGHT:
        x+= (speed + randomMovementOffset[0]) * GetFrameTime();
        y+= (speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    default:
        direction = STOP;
        break;
    }
}

cBall::~cBall(){
        instanceCount--;
}
