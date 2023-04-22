#pragma once
#ifndef CBALL_H
#define CBALL_H
#endif // CBALL_H
#include "defines.h"

class cBall {
private:
    float x, y;
    float originalX, originalY;
    int current_size, originalSize;
    float speed;
    eDir direction;
    float randomMovementOffset[2]{};
    std::mt19937 marsenneTwister{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> randomNrDistribution{1, 4};
    std::uniform_int_distribution<> randomNrDirection{1, 4};
public:
    cBall(int posX, int posY, int ball_size);
    void Reset();
    inline void changeDirection(eDir d);
    inline void randomDirection();
    inline int getX();
    inline int getY();
    inline eDir getDirection();
    inline void changeSize(int ball_size);
    inline int getSize();
    void randomizeMovement();
    void Move();
};

void cBall::changeDirection(eDir d){direction = d;}
void cBall::randomDirection(){direction = (eDir)(randomNrDirection(marsenneTwister));}
int cBall::getX(){return x;}
int cBall::getY(){return y;}
eDir cBall::getDirection(){return direction;}
void cBall::changeSize(int ball_size){current_size = ball_size;}
int cBall::getSize(){return current_size;}
