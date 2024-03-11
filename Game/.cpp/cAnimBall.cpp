#include "cAnimBall.h"

int cAnimBall::ballCount = 0;
extern int frame;

cAnimBall::cAnimBall(int posX, int posY, int ball_size, float ballSpeed) {
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    direction = STOP;
    originalSize = ball_size;
    current_size = ball_size;
    speed = ballSpeed;
    originalSpeed = speed;
    acceleration = 0;
    randomizeMovement();
    id = ballCount;
    ballCount++;
}
cAnimBall::~cAnimBall() {
    ballCount--;
}
void cAnimBall::Reset() {
    x = originalX;
    y = originalY;
    direction = STOP;
    current_size = originalSize;
    speed = originalSpeed;
}
void cAnimBall::randomizeMovement() {
    randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
    randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
}
void cAnimBall::Move() {
    if(direction != STOP) speed += speed * acceleration * GetFrameTime();
    switch (direction) {
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
void cAnimBall::Logic(double &startTimer, bool isPaused) {
    // Click to start the ball movement thing
    if(getDirection() == STOP) {
        startTimer = GetTime();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            setDirection(UPRIGHT);
        }
    }

    //Move only if game is NOT isPausedd
    if(!isPaused) {
        Move();
    }
}
void cAnimBall::Draw(cAnimBall *ball) {
    DrawCircle(ball->getX() - ball->getSize(), ball->getY() - ball->getSize(), ball->getSize(), WHITE);
}
void cAnimBall::Input() {
    //
}
