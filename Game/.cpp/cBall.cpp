#include "cBall.h"

int cBall::ballCount = 0;
Texture2D cBall::texBall;
extern int frame;

cBall::cBall(int posX, int posY, int ballSize, float ballSpeed) {
    setX(posX);
    setY(posY);
    originalX = x;
    originalY = y;
    setDirection(STOP);
    setSize(ballSize);
    originalSize = getSize();
    angle = 45.0f;
    speedX = abs(ballSpeed * cos(angle));
    speedY = abs(ballSpeed * sin(angle));
    originalSpeedX = speedX;
    originalSpeedY = speedY;
    acceleration = 0.017;
    randomizeMovement();
    ballCount++;
}
cBall::~cBall() {
    ballCount--;
}
void cBall::Reset() {
    x = originalX;
    y = originalY;
    direction = STOP;
    currentSize = originalSize;
    speedX = originalSpeedX;
    speedY = originalSpeedY;
}
void cBall::randomizeMovement() {
    randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
    randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
}
void cBall::Move() {
    float frameTime = fmod(GetFrameTime(), 1.0f);
    if(direction != STOP) {
        float speedLimit = 1000;
        if(speedY < abs(speedLimit * sin(angle))) {
            speedY += speedY * acceleration * frameTime;
        } else speedY += abs(speedLimit * sin(angle)) * frameTime;
        if(speedX < abs(speedLimit * cos(angle))) {
            speedX += speedX * acceleration * frameTime;
        } else speedX += abs(speedLimit * cos(angle))* frameTime;
        switch (direction) {
        case UPLEFT:
            x-=(speedX + randomMovementOffset[0]) * frameTime;
            y-=(speedY + randomMovementOffset[1]) * frameTime;
            break;
        case DOWNLEFT:
            x-=(speedX + randomMovementOffset[0]) * frameTime;
            y+=(speedY + randomMovementOffset[1]) * frameTime;
            break;
        case UPRIGHT:
            x+=(speedX + randomMovementOffset[0]) * frameTime;
            y-=(speedY + randomMovementOffset[1]) * frameTime;
            break;
        case DOWNRIGHT:
            x+=(speedX + randomMovementOffset[0]) * frameTime;
            y+=(speedY + randomMovementOffset[1]) * frameTime;
            break;
        default:
            direction = STOP;
            break;
        }
    }
}
