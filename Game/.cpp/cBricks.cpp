#include "cBricks.h"
#include "cPowerup.h"

extern int activePowerups;

Texture2D cBricks::texture[5];
int cBricks::brickCount = 0;

cBricks::cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType, int newAnimBallIndex) {
    ID = brickCount;
    animBallIndex = newAnimBallIndex;

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
    brickCount++;
}

cBricks::~cBricks() {
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

int cBricks::callOnCollision() {
    if(type > 0 && type < 4) { //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
        type -= 1;
    }
    if(type == 0 || type == 4) {
        enabled = 0;
        return 0;
    }
    return type;
}

void cBricks::Logic(cBall *ball, cPowerup *powerup, Sound soundBounceGeneral) {
    //Brick Collision
    Vector2 ball_collision = {ball->getX(), ball->getY()};
    Rectangle brick_collision;
    brick_collision = {x, y, brickWidth, brickHeight};
    if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && enabled) {
        if(!(activePowerups & (1 << 0))) {
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

            if(type > 0 && type < 4) { //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
                type -= 1;
            }
            if(type == 0 || type == 4) {
                enabled = 0;
            }
        } else {
            enabled = 0;
        }
        if(enabled == 0) {
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
