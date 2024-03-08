#ifndef CBALL_H
#define CBALL_H

#include "defines.h"

class cBall {
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
    static int ballCount;
    static Texture2D texBall;

    cBall(int posX, int posY, int ball_size, float ballSpeed);
    ~cBall();
    inline void setDirection(eDir d);
    inline void randomDirection();
    inline float getX() const;
    inline float getY() const;
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
    void Logic(double &startTimer);
};

void cBall::setDirection(eDir d) {
    direction = d;
}
void cBall::randomDirection() {
    direction = (eDir)(randomNrDirection(marsenneTwister));
}
float cBall::getX() const {
    return x;
}
float cBall::getY() const {
    return y;
}
void cBall::setX(float newX) {
    x = newX;
}
void cBall::setY(float newY) {
    y = newY;
}
eDir cBall::getDirection() const {
    return direction;
}
void cBall::setSize(int ball_size) {
    current_size = ball_size;
}
int cBall::getSize() const {
    return current_size;
}
int cBall::getAcceleration() const {
    return acceleration;
}
void cBall::setAcceleration(float newAccel = 0.017) {
    acceleration = newAccel;
}

#endif // CBALL_H
