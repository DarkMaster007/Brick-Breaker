#include "cBricks.h"
#include "cPowerup.h"

extern int activePowerups;
#ifdef _DEBUG
extern bool isPaused;
#endif // _DEBUG

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

void cBricks::Logic(cBall *ball, cPowerup *powerup, Sound soundBounce) {
    //Brick Collision
    Vector2 ball_collision = {ball[0].getX(), ball[0].getY()};
    if(CheckCollisionCircleRec(ball_collision, ball[0].getSize(), getDimensionsRec()) && enabled) {
        if(!(activePowerups & (1 << 0))) {
            if(ball[0].getX() <= x) {     //Left wall
                if(ball[0].getDirection() == UPRIGHT) {
                    ball[0].setDirection(UPLEFT);
                } else {
                    ball[0].setDirection(DOWNLEFT);
                }
                ball[0].setX(x - ball[0].getSize() - 1);
            } else if(ball[0].getX() >= x + brickWidth) {     //Right wall
                if(ball[0].getDirection() == UPLEFT) {
                    ball[0].setDirection(UPRIGHT);
                } else {
                    ball[0].setDirection(DOWNRIGHT);
                }
                ball[0].setX(x + brickWidth + ball[0].getSize() + 1);
            } else if(ball[0].getY() <= y) {      //Top wall
                if(ball[0].getDirection() == DOWNLEFT) {
                    ball[0].setDirection(UPLEFT);
                } else {
                    ball[0].setDirection(UPRIGHT);
                }
                ball[0].setY(y - ball[0].getSize() - 1);
            } else if(ball[0].getY() >= y + brickHeight) {       //Bottom wall
                if(ball[0].getDirection() == UPRIGHT) {
                    ball[0].setDirection(DOWNRIGHT);
                } else {
                    ball[0].setDirection(DOWNLEFT);
                }
                ball[0].setY(y + brickHeight + ball[0].getSize() + 1);
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
#ifdef _DEBUG
                    printf("Inside Brick Coord: %f %f\n", getX(), getY());
#endif // _DEBUG
                    powerup[j].spawnPowerup(this);
                    break;
                }
            }
        }

        PlaySound(soundBounce);
        ball[0].randomizeMovement();
    }
}
