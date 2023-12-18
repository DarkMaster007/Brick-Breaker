#pragma once
#ifndef CPADDLE_H
#define CPADDLE_H
#endif // CPADDLE_H

#include "defines.h"
#include "cBall.h"

class cPaddle {
  private:
    float x, y;
    float speed;
    float originalX, originalY;
    float paddleWidth, paddleHeight;
    float bounceReverseArea;           // How big the area is where the ball goes back instead of forward
    bool isColliding;
  public:
    static Texture2D textureBody;
    static Texture2D textureEdge_L;
    static Texture2D textureEdge_R;

    cPaddle(float posX, float posY, float width, float height);
    inline float getX();
    inline float getY();
    inline Vector2 getPosition();
    inline Vector2 getDimensions();
    inline Rectangle getDimensionsRec();
    inline float getSpeed();
    inline float getBounceReverseArea();
    void moveLeft();
    void moveLeft(float);
    void moveRight();
    void moveRight(float);
    void Reset();
    void Logic(cBall *ball, bool autoMove, bool isPaused, Sound soundBouncePaddle);
    static void Draw(cPaddle *paddle);
    void Input(bool autoMove, bool isPaused);
};

float cPaddle::getX() {
    return x;
}
float cPaddle::getY() {
    return y;
}
Vector2 cPaddle::getPosition() {
    return (Vector2) {
        getX(), getY()
    };
}
Vector2 cPaddle::getDimensions() {
    return {paddleWidth, paddleHeight};
}
Rectangle cPaddle::getDimensionsRec() {
    return {getX(), getY(), getDimensions().x, getDimensions().y};
}
float cPaddle::getSpeed() {
    return speed;
}
float cPaddle::getBounceReverseArea() {
    return bounceReverseArea;
}
