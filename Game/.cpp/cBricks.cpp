#include "cBricks.h"
#include "cPowerup.h"

Texture2D cBricks::texture[5];

int cBricks::brickCount = 0;

cBricks::cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType) {
    ID = brickCount;

    //X clamping
    if(loadedX < 10) {
        loadedX += 50;
        if(loadedX < 10) {
            loadedX = 11;
        }
    }
    if(loadedX + loadedWidth > GetScreenWidth() - 10) {
        loadedX -= 50;
        if(loadedX < 10) {
            loadedX = GetScreenWidth() - 11 - loadedWidth;
        }
    }
    //

    //Y clamping
    if(loadedY + loadedHeight > GetScreenHeight() - 350) {
        loadedY -= 25;
        if(loadedY > GetScreenHeight() - 350) {
            loadedY = GetScreenHeight() - 351 - loadedHeight;
        }
    }
    if(loadedY < 0) {
        loadedY += 50;
        if(loadedY < 0) {
            loadedY = 11;
        }
    }
    //

    x = loadedX;
    y = loadedY;
    brickWidth = loadedWidth;
    brickHeight = loadedHeight;
    type = loadedType;
    enabled = true;

    if(type == 4) {
        int ballSize = 3;
        int ballSpeed = 50;
        animationBalls = new cAnimBall[STANDARD_ANIM_BALL_COUNT] {
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed},
            {GetRandomValue(x + 6, x + brickWidth - 6), GetRandomValue(y + 6, y + brickHeight - 6), ballSize, ballSpeed}
        };
        for(int i = 0; i < STANDARD_ANIM_BALL_COUNT; i++) {
            animationBalls[i].randomDirection();
        }
    } else animationBalls = nullptr;

    brickCount++;
}

cBricks::~cBricks() {
    if(animationBalls != nullptr)
        delete[] animationBalls;
    brickCount--;
}

Color cBricks::getColor() {
    switch(type) {
    case 1:
        return WHITE;
    case 2:
        return BEIGE;
    case 3:
        return GRAY;
    case 4:
        return ORANGE;
    case 5:
        return GOLD;
    }
    return BLACK;
}

void cBricks::Logic(cBall *ball, cPowerup *powerup, Sound soundBounceGeneral) {
    //Brick Collision
    Vector2 ball_collision = {ball->getX(), ball->getY()};
    Rectangle brick_collision;
    brick_collision = {x, y, brickWidth, brickHeight};
    if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && enabled) {
        if(ball->getX() <= x) {
            if(ball->getDirection() == UPRIGHT) {
                ball->setDirection(UPLEFT);
            } else {
                ball->setDirection(DOWNLEFT);
            }
        } else if(ball->getX() >= x + brickWidth) {
            if(ball->getDirection() == UPLEFT) {
                ball->setDirection(UPRIGHT);
            } else {
                ball->setDirection(DOWNRIGHT);
            }
        } else if(ball->getY() <= y) {
            if(ball->getDirection() == DOWNLEFT) {
                ball->setDirection(UPLEFT);
            } else {
                ball->setDirection(UPRIGHT);
            }
        } else if(ball->getY() >= y + brickHeight) {
            if(ball->getDirection() == UPRIGHT) {
                ball->setDirection(DOWNRIGHT);
            } else {
                ball->setDirection(DOWNLEFT);
            }
        }
        if(type == 4) {
            type = 0;
        } else if(type != 5) {   //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
            type -= 1;
        }
        if(type == 0) {
            enabled = 0;
            for(int j = 0; j < MAX_POWERUPS; j++) {
                if(!powerup[j].getEnabled()) {
                    powerup[j].spawnPowerup(this);
                    break;
                }
            }
        }

        PlaySound(soundBounceGeneral);
        ball->randomizeMovement();
    }
}

void cBricks::Draw(cBricks *brick, float animationFrame) {
    for(int i = 0; i < brickCount - 1; i++) {
        if(brick[i].enabled) {
            if(brick[i].type < 4) {
                brick[i].drawBrickPulse((int)animationFrame, brick[i].type);
            } else if(brick[i].type == 4) {
                brick[i].drawBrickBallBounce((int)animationFrame, 1, brick[i].getDimensionsRec());
            } else
                DrawRectangle(brick[i].x, brick[i].y, brick[i].brickWidth, brick[i].brickHeight, brick[i].getColor());
#ifdef _DEBUG
            //Draw brick number on bricks if DEBUG:
            DrawText(TextFormat("%d", i+1), brick[i].x + brick[i].brickWidth / 2 - 2.5, brick[i].y + brick[i].brickHeight / 2 - 5, 5, RED);
#endif // _DEBUG
        }
    }
}

void cBricks::drawBrickPulse(int i, int lineAmount) {
    Rectangle rec;
    //Calculate ratios used to make time to shrink similar between width and height
    float calcHoriz = brickHeight / brickWidth;
    float calcVert = brickWidth / brickHeight;
    float stepX = (brickWidth * 0.5) / (lineAmount + 1);
    float stepY = (brickHeight * 0.5) / (lineAmount + 1);

    if(calcHoriz <= 0) calcHoriz = 1;
    if(calcVert <= 0) calcVert = 1;
    if(calcVert > calcHoriz) calcHoriz = 1;
    else calcVert = 1;

    float offsetX, offsetY;

    for(int kk = 0; kk < lineAmount; kk++) {
        offsetX = fmod((stepX * kk + (i + kk) / calcHoriz), brickWidth / 2);
        offsetY = fmod((stepY * kk + (i + kk) / calcVert), brickHeight / 2);

        rec.x = x + offsetX;
        rec.y = y + offsetY;
        rec.width = brickWidth - offsetX * 2;
        rec.height = brickHeight - offsetY * 2;

        DrawRectangleLinesEx(rec, 1.5, ORANGE);
    }

    //Draw first rectangle
    DrawRectangleRoundedLines({x, y, brickWidth, brickHeight}, 0.1, 1000, 3, ORANGE);
}

void cBricks::drawBrickBallBounce(int i, int lineAmount, Rectangle drawnRectangle) {
    Rectangle rec;
    float calcHoriz, calcVert;

    //Calculate x and y
    int limitX = (drawnRectangle.x + drawnRectangle.width - lineAmount);
    int limitY = (drawnRectangle.y + drawnRectangle.height - lineAmount);
    if(animationBalls[0].ballCount % STANDARD_ANIM_BALL_COUNT != 0) throw;
    for(int kk = 0; kk < STANDARD_ANIM_BALL_COUNT; kk++) {
        if(animationBalls[kk].getX() > limitX) {
            if(animationBalls[kk].getDirection() == DOWNRIGHT) {
                animationBalls[kk].setDirection(DOWNLEFT);
            } else animationBalls[kk].setDirection(UPLEFT);
        }
        if(animationBalls[kk].getX() < (drawnRectangle.x + 9)) {
            if(animationBalls[kk].getDirection() == DOWNLEFT) {
                animationBalls[kk].setDirection(DOWNRIGHT);
            } else animationBalls[kk].setDirection(UPRIGHT);
        }
        if(animationBalls[kk].getY() > limitY) {
            if(animationBalls[kk].getDirection() == DOWNRIGHT) {
                animationBalls[kk].setDirection(UPRIGHT);
            } else animationBalls[kk].setDirection(UPLEFT);
        }
        if(animationBalls[kk].getY() < (drawnRectangle.y + 9)) {
            if(animationBalls[kk].getDirection() == UPRIGHT) {
                animationBalls[kk].setDirection(DOWNRIGHT);
            } else animationBalls[kk].setDirection(DOWNLEFT);
        }
        animationBalls[kk].Move();
        //DrawCircle
        cAnimBall::Draw(&animationBalls[kk]);
    }
    //Draw first rectangle
    DrawRectangleRoundedLines(drawnRectangle, 0.1, 1000, 3, ORANGE);
}
