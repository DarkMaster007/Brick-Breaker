#include "cBall.h"
#include "defines.h"

Texture2D cBall::texBall;

cBall::cBall(int posX, int posY, int ball_size)
{
    Image imgBall = LoadImage(TEX_BALL);
    ImageResize(&imgBall, ball_size * 2, ball_size * 2);
    texBall = LoadTextureFromImage(imgBall);

    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    direction = STOP;
    originalSize = ball_size;
    current_size = ball_size;
    speed = 300;
    randomizeMovement();
}
void cBall::Reset()
{
    x = originalX;
    y = originalY;
    direction = STOP;
    current_size = originalSize;
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
        x-=(speed + randomMovementOffset[0]) * GetFrameTime();
        y-=(speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case DOWNLEFT:
        x-=(speed + randomMovementOffset[0]) * GetFrameTime();
        y+=(speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case UPRIGHT:
        x+=(speed + randomMovementOffset[0]) * GetFrameTime();
        y-=(speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    case DOWNRIGHT:
        x+=(speed + randomMovementOffset[0]) * GetFrameTime();
        y+=(speed + randomMovementOffset[1]) * GetFrameTime();
        break;
    default:
        direction = STOP;
        break;
    }
}
