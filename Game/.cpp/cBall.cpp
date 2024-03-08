#include "cBall.h"

int cBall::ballCount = 0;
Texture2D cBall::texBall;
extern int frame;

cBall::cBall(int posX, int posY, int ball_size, float ballSpeed) {
    setX(posX);
    setY(posY);
    originalX = x;
    originalY = y;
    setDirection(STOP);
    setSize(ball_size);
    originalSize = getSize();
    speed = ballSpeed;
    originalSpeed = speed;
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
    current_size = originalSize;
    speed = originalSpeed;
}
void cBall::randomizeMovement() {
    randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
    randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
}
void cBall::Move() {
    float frameTime = fmod(GetFrameTime(), 1.0f);
    if(direction != STOP) speed += speed * acceleration * frameTime;
    switch (direction) {
    case STOP:
        break;
    case UPLEFT:
        x-=(speed + randomMovementOffset[0]) * frameTime;
        y-=(speed + randomMovementOffset[1]) * frameTime;
        break;
    case DOWNLEFT:
        x-=(speed + randomMovementOffset[0]) * frameTime;
        y+=(speed + randomMovementOffset[1]) * frameTime;
        break;
    case UPRIGHT:
        x+=(speed + randomMovementOffset[0]) * frameTime;
        y-=(speed + randomMovementOffset[1]) * frameTime;
        break;
    case DOWNRIGHT:
        x+=(speed + randomMovementOffset[0]) * frameTime;
        y+=(speed + randomMovementOffset[1]) * frameTime;
        break;
    default:
        direction = STOP;
        break;
    }
}
void cBall::Logic(double &startTimer) {
    // Click to start the ball movement thing
    if(getDirection() == STOP) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            setDirection(UPRIGHT);
        }
    }
    Move();
}
