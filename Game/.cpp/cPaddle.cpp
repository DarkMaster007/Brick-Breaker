#include "cPaddle.h"

Texture2D cPaddle::textureBody;
Texture2D cPaddle::textureEdge_L;
Texture2D cPaddle::textureEdge_R;
extern int frame;

cPaddle::cPaddle(float posX, float posY, float width, float height) {
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    paddleWidth = width;
    paddleHeight = height;
    bounceReverseArea = 13;
    speed = 15;

    if(textureBody.id == 0 && textureEdge_L.id == 0 && textureEdge_R.id == 0) {
        textureBody = LoadTexture(TEX_PADDLE_BODY);
        textureEdge_L = LoadTexture(TEX_PADDLE_EDGE_L);
        textureEdge_R = LoadTexture(TEX_PADDLE_EDGE_R);
    }
}

void cPaddle::Reset() {
    x = originalX;
    y = originalY;
}

void cPaddle::moveLeft() {
    x -= speed + GetFrameTime();
}

void cPaddle::moveLeft(float step) {
    x -= step;
}

void cPaddle::moveRight() {
    x += speed + GetFrameTime();
}

void cPaddle::moveRight(float step) {
    x += step;
}
void cPaddle::Logic(cBall *ball, bool autoMove, Sound soundBouncePaddle) {
    //Paddle Collision
    Vector2 ball_collision = {ball->getX(), ball->getY()};
    bool collisionStatus = CheckCollisionCircleRec(ball_collision,ball->getSize(), getDimensionsRec());
    if(collisionStatus) {
        if(getDimensions().x > 30) {
            if(ball->getX() < getX() + getBounceReverseArea()) {
                ball->setDirection(UPLEFT);
            }
            if(ball->getX() > getX() + getDimensions().x - getBounceReverseArea()) {
                ball->setDirection(UPRIGHT);
            }
            if(ball->getX() >= getX() + getBounceReverseArea() && ball->getX() <= getX() + getDimensions().x - getBounceReverseArea()) {
                if(ball->getDirection() == DOWNLEFT) {
                    ball->setDirection(UPLEFT);
                } else {
                    ball->setDirection(UPRIGHT);
                }
            }
        } else {
            if(ball->getDirection() == DOWNLEFT) {
                ball->setDirection(UPLEFT);
            } else ball->setDirection(UPRIGHT);
        }
        isColliding = true;
    } else if(isColliding) {
        PlaySound(soundBouncePaddle);
        isColliding = false;
    }

    //Auto-movement for the paddle (works fine but it's jittery)
    if(autoMove) {
        if(getX() - getSpeed() > 10) {
            if(ball->getX() < getX() + 25) {
                moveLeft();
            }
        }
        if(getX() + getDimensions().x + getSpeed() < GetScreenWidth() - 10) {
            if(ball->getX() > getX() + 25) {
                moveRight();
            }
        }
    }
}
void cPaddle::Input(bool autoMove, bool isPaused) {
    //Paddle movement
    if(autoMove || isPaused) return;
    if(GetMouseX() - getSpeed() <= 10) return;
    if(GetMouseX() + getDimensions().x / 2 + getSpeed() >= GetScreenWidth() - 10) return;
    setX(GetMouseX());
}
