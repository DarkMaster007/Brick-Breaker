#pragma once
#ifndef CBRICKS_H
#define CBRICKS_H
#endif // CBRICKS_H

#include "defines.h"
#include "cBall.h"
#include "cAnimBall.h"

class cPowerup;

class cBricks {
  private:
    float x, y;
    float brickWidth;
    float brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    bool enabled;
  public:
    static Texture2D texture[5];
    static int brickCount;

    int ID;
    cAnimBall *animationBalls;

    cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType);
    ~cBricks();
    Color getColor();
    void Reset();
    void Logic(cBall *ball, cPowerup *powerup, Sound soundBounceGeneral);
    static void Draw(cBricks *brick, float animFrame);
    void drawBrickPulse(int i, int lineAmount);
    void drawBrickBallBounce(int i, int lineAmount, Rectangle drawnRectangle);
    inline float getX();
    inline float getY();
    inline Vector2 getDimensions();
    inline Rectangle getDimensionsRec();
    inline void setX(float);
    inline void setY(float);
    inline void setDimensions(float, float);
    inline void setDimensionsRec(Rectangle);
    inline int getType();
    inline void setType(int);
    inline bool getEnabled();
    inline void setEnabled(bool);
};

float cBricks::getX() {
    return x;
}
float cBricks::getY() {
    return y;
}
Vector2 cBricks::getDimensions() {
    return {brickWidth, brickHeight};
}
Rectangle cBricks::getDimensionsRec() {
    return {getX(), getY(), getDimensions().x, getDimensions().y};
}
void cBricks::setX(float newX) {
    x = newX;
}
void cBricks::setY(float newY) {
    y = newY;
}
void cBricks::setDimensions(float newWidth, float newHeight) {
    brickWidth = newWidth;
    brickHeight = newHeight;
}
void cBricks::setDimensionsRec(Rectangle newRectangle) {
    setX(newRectangle.x);
    setY(newRectangle.y);
    setDimensions(newRectangle.width, newRectangle.height);
}
int cBricks::getType() {
    return type;
}
void cBricks::setType(int newType) {
    type = newType;
}
bool cBricks::getEnabled() {
    return enabled;
}
void cBricks::setEnabled(bool newEnabled) {
    enabled = newEnabled;
}
