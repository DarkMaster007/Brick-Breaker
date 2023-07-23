#include "cPaddle.h"

Texture2D cPaddle::textureBody;
Texture2D cPaddle::textureEdge_L;
Texture2D cPaddle::textureEdge_R;

cPaddle::cPaddle(int posX, int posY, int width, int height) {
    originalX = posX;
    originalY = posY;
    x = posX;
    y = posY;
    paddle_width = width;
    paddle_height = height;
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

void cPaddle::moveLeft(int a) {
    x -= a;
}

void cPaddle::moveRight() {
    x += speed + GetFrameTime();
}

void cPaddle::moveRight(int a) {
    x += a;
}
void cPaddle::Logic(cBall *ball, bool autoMove, bool isPaused, Sound soundBouncePaddle) {
    //Paddle Collision
    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};
    Rectangle paddle_collision = {(float)getX(), (float)getY(), getSize().x, getSize().y};
    bool collisionStatus = CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision);
    if(collisionStatus) {
        if(getSize().x > 30) {
            if(ball->getX() < getX() + getBounceReverseArea()) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getX() > getX() + getSize().x - getBounceReverseArea()) {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getX() >= getX() + getBounceReverseArea() && ball->getX() <= getX() + getSize().x - getBounceReverseArea()) {
                if(ball->getDirection() == DOWNLEFT) {
                    ball->changeDirection(UPLEFT);
                } else {
                    ball->changeDirection(UPRIGHT);
                }
            }
        } else {
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(UPLEFT);
            } else ball->changeDirection(UPRIGHT);
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
        if(getX() + getSize().x + getSpeed() < GetScreenWidth() - 10) {
            if(ball->getX() > getX() + 25) {
                moveRight();
            }
        }
    }
}
void cPaddle::Draw(cPaddle *paddle) {
    DrawRectangle(paddle->getX(),paddle->getY(),paddle->getSize().x,paddle->getSize().y,WHITE);

    DrawRectangle(paddle->getX(), paddle->getY(), paddle->getBounceReverseArea(), paddle->getSize().y, YELLOW);
    DrawRectangle(paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea(), paddle->getY(), paddle->getBounceReverseArea(), paddle->getSize().y, YELLOW);

    Rectangle Source = {0, 0, (float)paddle->textureEdge_L.width, (float)paddle->textureEdge_L.height};
    Rectangle Destination = {(float)paddle->getX(), (float)paddle->getY(), (float)paddle->getBounceReverseArea(), paddle->getSize().y};
    DrawTexturePro(paddle->textureEdge_L, Source, Destination, {0,0}, 0, WHITE);

    Source = {0, 0, (float)paddle->textureEdge_R.width, (float)paddle->textureEdge_R.height};
    Destination = {(float)(paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea()), (float)paddle->getY(), (float)paddle->getBounceReverseArea(), paddle->getSize().y};
    DrawTexturePro(paddle->textureEdge_R, Source, Destination, {0,0}, 0, WHITE);

    Source = {0, 0, (float)paddle->textureBody.width, (float)paddle->textureBody.height};
    Destination = {(float)(paddle->getX() + paddle->getBounceReverseArea()), (float)paddle->getY(), paddle->getSize().x -  2 * paddle->getBounceReverseArea(), paddle->getSize().y};
    DrawTexturePro(paddle->textureBody, Source, Destination, {0,0}, 0, WHITE);
}
void cPaddle::Input(bool autoMove, bool isPaused) {
    //Paddle movement
    if(!autoMove && !isPaused) {
        if(getX() - getSpeed() >= 10) {
            if(getX() + getSize().x + getSpeed() <= GetScreenWidth() - 10) {
                if(GetMouseDelta().x < 0 || IsKeyDown('A')) {
                    moveLeft();
                }
                if(GetMouseDelta().x > 0 || IsKeyDown('D')) {
                    moveRight();
                }
            } else moveLeft(1);
        } else moveRight(1);
    }
    //
}
