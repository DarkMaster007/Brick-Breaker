#include "cBall.h"

int cBall::ballCount = 0;
Texture2D cBall::texBall;
extern int frame;

cBall::cBall(int posX, int posY, int ball_size, float ballSpeed) {
    //Image imgBall = LoadImage(TEX_BALL);
    //ImageResize(&imgBall, ball_size * 2, ball_size * 2);
    //texBall = LoadTextureFromImage(imgBall);

    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    direction = STOP;
    originalSize = ball_size;
    current_size = ball_size;
    speed = ballSpeed;
    originalSpeed = speed;
    acceleration = 0.017;
    randomizeMovement();
    ballCount++;
}
cBall::~cBall(){
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
void cBall::Logic(double &startTimer, bool isPaused) {
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
void cBall::Draw(cBall *ball) {
    //DrawTexture(texBall, ball->getX() - ball->getSize(), ball->getY() - ball->getSize(), WHITE);
    DrawCircle(ball->getX(), ball->getY(), ball->getDimensions(), WHITE);
}
void cBall::Input() {
    //
}
