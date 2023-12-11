#pragma once
#ifndef CBALL_H
#define CBALL_H
#endif // CBALL_H
#include "defines.h"

class cAnimBall {
  private:
    float x, y;
    float originalX, originalY;
    int current_size;
    int originalSize;
    float speed;
    float originalSpeed;
    float acceleration;
    eDir direction;
    float randomMovementOffset[2] {};
    std::mt19937 marsenneTwister{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> randomNrDistribution{1, 4};
    std::uniform_int_distribution<> randomNrDirection{1, 4};
  public:
    int id;
    static int ballCount;

    cAnimBall(int posX, int posY, int ball_size, float ballSpeed);
    ~cAnimBall();
    inline void setDirection(eDir d);
    inline void randomDirection();
    inline float getX();
    inline float getY();
    inline void setX(float newX);
    inline void setY(float newY);
    inline eDir getDirection();
    inline void setSize(int ball_size);
    inline int getSize();
    inline int getAcceleration();
    inline void setAcceleration(float newAccel);
    void randomizeMovement();
    void Move();
    void Reset();
    void Logic(double &startTimer, bool isPaused);
    static void Draw(cAnimBall *ball);
    void Input();
};

void cAnimBall::setDirection(eDir d) {
    direction = d;
}
void cAnimBall::randomDirection() {
    direction = (eDir)(randomNrDirection(marsenneTwister));
}
float cAnimBall::getX() {
    return x;
}
float cAnimBall::getY() {
    return y;
}
void cAnimBall::setX(float newX) {
    x = newX;
}
void cAnimBall::setY(float newY) {
    y = newY;
}
eDir cAnimBall::getDirection() {
    return direction;
}
void cAnimBall::setSize(int ball_size) {
    current_size = ball_size;
}
int cAnimBall::getSize() {
    return current_size;
}
int cAnimBall::getAcceleration() {
    return acceleration;
}
void cAnimBall::setAcceleration(float newAccel = 0.017) {
    acceleration = newAccel;
}
