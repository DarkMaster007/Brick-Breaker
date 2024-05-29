#ifndef CANIMBALL_H
#define CANIMBALL_H

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
    inline float getX() const;
    inline float getY() const;
    inline Vector2 getV() const;
    inline void setX(float newX);
    inline void setY(float newY);
    inline eDir getDirection() const;
    inline void setSize(int ball_size);
    inline int getSize() const;
    inline int getAcceleration() const;
    inline void setAcceleration(float newAccel);
    void randomizeMovement();
    void Move();
    void Reset();
    void Logic(double &startTimer, bool isPaused);
    static void Draw(cAnimBall *ball, Color ballColor);
    void Input();
};

void cAnimBall::setDirection(eDir d) {
    direction = d;
}
void cAnimBall::randomDirection() {
    direction = (eDir)(randomNrDirection(marsenneTwister));
}
float cAnimBall::getX() const {
    return x;
}
float cAnimBall::getY() const {
    return y;
}
Vector2 cAnimBall::getV() const {
    return {x, y};
}
void cAnimBall::setX(float newX) {
    x = newX;
}
void cAnimBall::setY(float newY) {
    y = newY;
}
eDir cAnimBall::getDirection() const {
    return direction;
}
void cAnimBall::setSize(int ball_size) {
    current_size = ball_size;
}
int cAnimBall::getSize() const {
    return current_size;
}
int cAnimBall::getAcceleration() const {
    return acceleration;
}
void cAnimBall::setAcceleration(float newAccel = 0.017) {
    acceleration = newAccel;
}

#endif // CANIMBALL_H
