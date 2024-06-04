#ifndef CBRICKS_H
#define CBRICKS_H

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
    int animBallIndex;

    cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType, int newAnimBallIndex);
    ~cBricks();
    Color getColor();
    void Reset();
    void Logic(cBall *ball, cPowerup *powerup, Sound soundBounceGeneral);
    static void Draw(cBricks *brick, float animFrame);
    int callOnCollision();
    inline float getX() const;
    inline float getY() const;
    inline Vector2 getDimensions() const;
    inline Rectangle getDimensionsRec() const;
    inline void setX(float);
    inline void setY(float);
    inline void setDimensions(float, float);
    inline void setDimensionsRec(Rectangle);
    inline int getType() const;
    inline void setType(int);
    inline bool getEnabled() const;
    inline void setEnabled(bool);
};

float cBricks::getX() const {
    return x;
}
float cBricks::getY() const {
    return y;
}
Vector2 cBricks::getDimensions() const {
    return {brickWidth, brickHeight};
}
Rectangle cBricks::getDimensionsRec() const {
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
int cBricks::getType() const {
    return type;
}
void cBricks::setType(int newType) {
    type = newType;
}
bool cBricks::getEnabled() const {
    return enabled;
}
void cBricks::setEnabled(bool newEnabled) {
    enabled = newEnabled;
}

#endif // CBRICKS_H
