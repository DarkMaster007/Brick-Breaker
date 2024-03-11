#ifndef CPADDLE_H
#define CPADDLE_H

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
    inline float getX() const;
    inline float getY() const;
    inline void setX(float newX);
    inline void setY(float newY);
    inline Vector2 getPosition() const;
    inline Vector2 getDimensions() const;
    inline Rectangle getDimensionsRec() const;
    inline float getSpeed() const;
    inline float getBounceReverseArea() const;
    void moveLeft();
    void moveLeft(float);
    void moveRight();
    void moveRight(float);
    void Reset();
    void Logic(cBall *ball, bool autoMove, Sound soundBouncePaddle);
    void Input(bool autoMove, bool isPaused);
};

float cPaddle::getX() const {
    return x;
}
float cPaddle::getY() const {
    return y;
}
void cPaddle::setX(float newX) {
    x = newX;
}
void cPaddle::setY(float newY) {
    y = newY;
}
Vector2 cPaddle::getPosition() const {
    return (Vector2) {
        getX(), getY()
    };
}
Vector2 cPaddle::getDimensions() const {
    return {paddleWidth, paddleHeight};
}
Rectangle cPaddle::getDimensionsRec() const {
    return {getX(), getY(), getDimensions().x, getDimensions().y};
}
float cPaddle::getSpeed() const {
    return speed;
}
float cPaddle::getBounceReverseArea() const {
    return bounceReverseArea;
}

#endif // CPADDLE_H
