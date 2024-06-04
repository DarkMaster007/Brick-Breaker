#include "cPaddle.h"

Texture2D cPaddle::textureBody;
Texture2D cPaddle::textureEdge_L;
Texture2D cPaddle::textureEdge_R;
extern int frame;
extern char rootPath[2048];

cPaddle::cPaddle(float posX, float posY, float width, float height) {
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    paddleWidth = width;
    paddleHeight = height;
    originalPaddleWidth = paddleWidth;
    bounceReverseArea = 13;
    speed = 15;
    minPaddleWidth = bounceReverseArea * 3.5;

    if(textureBody.id == 0 && textureEdge_L.id == 0 && textureEdge_R.id == 0) {
        textureBody = LoadTexture(TextFormat("%s%s", rootPath,TEX_PADDLE_BODY));
        textureEdge_L = LoadTexture(TextFormat("%s%s", rootPath,TEX_PADDLE_EDGE_L));
        textureEdge_R = LoadTexture(TextFormat("%s%s", rootPath,TEX_PADDLE_EDGE_R));
    }
}

void cPaddle::Reset() {
    x = originalX;
    y = originalY;
    paddleWidth = originalPaddleWidth;
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
    Vector2 ball_collision = {ball[0].getX(), ball[0].getY()};
    bool collisionStatus = CheckCollisionCircleRec(ball_collision, ball[0].getSize(), getDimensionsRec());
    if(collisionStatus) {
        if(getDimensions().x > 30) {
            if(ball[0].getX() < getX() + getBounceReverseArea()) {
                ball[0].setDirection(UPLEFT);
            }
            if(ball[0].getX() > getX() + getDimensions().x - getBounceReverseArea()) {
                ball[0].setDirection(UPRIGHT);
            }
            if(ball[0].getX() >= getX() + getBounceReverseArea() && ball[0].getX() <= getX() + getDimensions().x - getBounceReverseArea()) {
                if(ball[0].getDirection() == DOWNLEFT) {
                    ball[0].setDirection(UPLEFT);
                } else {
                    ball[0].setDirection(UPRIGHT);
                }
            }
        } else {
            if(ball[0].getDirection() == DOWNLEFT) {
                ball[0].setDirection(UPLEFT);
            } else ball[0].setDirection(UPRIGHT);
        }
        isColliding = true;
    } else if(isColliding) {
        PlaySound(soundBouncePaddle);
        isColliding = false;
    }

    //Auto-movement for the paddle (works fine but it's jittery)
    if(autoMove) {
        if(getX() - getSpeed() > 10) {
            if(ball[0].getX() < getX() + 25) {
                moveLeft();
            }
        }
        if(getX() + getDimensions().x + getSpeed() < GetScreenWidth() - 10) {
            if(ball[0].getX() > getX() + 25) {
                moveRight();
            }
        }
    }
}
void cPaddle::Input(bool autoMove, bool isPaused) {
    //Paddle movement
    float newPosX = GetMouseX();
#ifdef _DEBUG
    if((int)GetMouseDelta().x != 0)
        printf("Mouse XPos: %f\n", newPosX);
#endif // _DEBUG
    if(autoMove || isPaused) return;
    if(newPosX - 1 <= 10) newPosX = 15;
    if(newPosX + paddleWidth + 1 >= GetScreenWidth() - 10) newPosX = GetScreenWidth() - 10 - 15 - paddleWidth;
    setX(newPosX);
}
