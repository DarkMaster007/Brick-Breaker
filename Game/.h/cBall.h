#ifndef CBALL_H
#define CBALL_H

#include "defines.h"
#include <chrono>
#include <random>

class cBall {
  private:
    float x, y;
    float originalX, originalY;
    int currentSize;
    int originalSize;
    float angle;
    float speedX, speedY;
    float originalSpeedX, originalSpeedY;
    float acceleration;
    eDir direction;
    float randomMovementOffset[2] {};
    std::mt19937 marsenneTwister{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> randomNrDistribution{1, 4};
    std::uniform_int_distribution<> randomNrDirection{1, 4};

  public:
    static int ballCount;
    static Texture2D texBall;

    cBall(int posX, int posY, int ball_size, float ballSpeed, eDir direction);
    cBall(int posX, int posY, int ball_size, float ballSpeed);
    cBall(const cBall& other);
    cBall(cBall&& other) noexcept;
    cBall& operator=(const cBall&) = delete;
    cBall& operator=(cBall&& other) noexcept;
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
    inline float getAcceleration() const;
    inline void setAcceleration(float newAccel);
    inline void setSpeed(float newSpeed);
    inline void resetSize();
    void randomizeMovement();
    void Move();
    void Reset();
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
    if(newX <= 10 + currentSize) {
        x = 11 + currentSize;
    } else {
        if(newX >= GetScreenWidth() - 10 - currentSize) {
            x = GetScreenWidth() - 11 - currentSize;
        } else {
            x = newX;
        }
    }
}

void cBall::setY(float newY) {
    y = newY;
    if(newY <= 10 + currentSize) {
        y = 11 + currentSize;
    } else {
        if(newY >= GetScreenHeight() - 10 - currentSize) {
            y = GetScreenHeight() - 11 - currentSize;
        } else {
            y = newY;
        }
    }
}

eDir cBall::getDirection() const {
    return direction;
}

void cBall::setSize(int ballSize) {
    currentSize = ballSize >= 5 ? ballSize : 5;
}

int cBall::getSize() const {
    return currentSize;
}

float cBall::getAcceleration() const {
    return acceleration;
}

void cBall::setAcceleration(float newAccel) {
    acceleration = newAccel;
}

void cBall::setSpeed(float newSpeed) {
    speedX = std::abs(newSpeed * std::cos(angle));
    speedY = std::abs(newSpeed * std::sin(angle));
}

void cBall::resetSize() {
    currentSize = originalSize;
}

#endif // CBALL_H
