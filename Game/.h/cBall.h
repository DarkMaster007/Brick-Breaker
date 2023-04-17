#pragma once
#ifndef CBALL_H
#define CBALL_H

#include "defines.h"

class cBall
{
private:
    float x, y;
    int originalX, originalY;
    int current_size, originalSize;
    eDir direction;
    float speed{288};
    float randomMovementOffset[2] {};
    std::mt19937 marsenneTwister{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> randomNrDistribution{1, 5};
    std::uniform_int_distribution<> randomNrDirection{1, 4};
public:
    static int instanceCount;

    cBall(int posX, int posY, int ball_size);
    void Reset();
    void changeDirection(eDir d);
    void randomDirection();
    int getX();
    int getY();
    eDir getDirection();
    void changeSize(int ball_size);
    int getSize();
    void setSpeed(float speed);
    void randomizeMovement();
    void Move();
    ~cBall();
};

#endif // POWERUP_H
