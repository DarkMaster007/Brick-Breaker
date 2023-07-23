#include "cBricks.h"
#include "cPowerup.h"

Texture2D cBricks::texture;

int cBricks::brickCount = 0;

cBricks::cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType) {
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

    position.x = loadedX;
    position.y = loadedY;
    brickWidth = loadedWidth;
    brickHeight = loadedHeight;
    type = loadedType;
    enabled = true;
    if(texture.id == 0) {
        Image imgBrick = LoadImage(TEX_BRICKS);
        ImageResize(&imgBrick, 100*10, 70*10);
        texture = LoadTextureFromImage(imgBrick);
    }
}

Color cBricks::getColor() {
    switch(type) {
    case 1:
        return SKYBLUE;
    case 2:
        return BLUE;
    case 3:
        return GRAY;
    case 4:
        return ORANGE;
    case 5:
        return GOLD;
    }
}
void cBricks::Logic(cBricks *brick, cBall *ball, cPowerup *powerup, Sound soundBounceGeneral) {
    //Brick Collision
    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};
    Rectangle brick_collision;
    for(int i = 0; i < cBricks::brickCount - 1; i++) {
        brick_collision = {brick[i].position.x, brick[i].position.y, (float)brick[i].brickWidth, (float)brick[i].brickHeight};
        if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && brick[i].enabled) {
            if(ball->getX() <= brick[i].position.x) {
                if(ball->getDirection() == UPRIGHT) {
                    ball->changeDirection(UPLEFT);
                } else {
                    ball->changeDirection(DOWNLEFT);
                }
            } else if(ball->getX() >= brick[i].position.x + brick[i].brickWidth) {
                if(ball->getDirection() == UPLEFT) {
                    ball->changeDirection(UPRIGHT);
                } else {
                    ball->changeDirection(DOWNRIGHT);
                }
            } else if(ball->getY() <= brick[i].position.y) {
                if(ball->getDirection() == DOWNLEFT) {
                    ball->changeDirection(UPLEFT);
                } else {
                    ball->changeDirection(UPRIGHT);
                }
            } else if(ball->getY() >= brick[i].position.y + brick[i].brickHeight) {
                if(ball->getDirection() == UPRIGHT) {
                    ball->changeDirection(DOWNRIGHT);
                } else {
                    ball->changeDirection(DOWNLEFT);
                }
            }
            if(brick[i].type == 4) {
                brick[i].type = 0;
            } else if(brick[i].type != 5) {   //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
                brick[i].type -= 1;
            }
            if(brick[i].type == 0) {
                brick[i].enabled = 0;
                for(int j = 0; j < 6; j++) {
                    if(!powerup[j].getEnabled()) {
                        powerup[j].spawnPowerup(brick[i]);
                        break;
                    }
                    i++;
                }
            }

            PlaySound(soundBounceGeneral);
            ball->randomizeMovement();
        }
    }
}
void cBricks::Draw(cBricks *brick) {
    for(int i = 0; i < brickCount - 1; i++) {
        if(brick[i].enabled) {
            DrawTexturePro(texture, Rectangle{0, 0, texture.width, texture.height}, Rectangle{brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight},Vector2{0,0}, 0.0f, brick[i].getColor());

            //Draw brick number on bricks if DEBUG:
#ifdef _DEBUG
            DrawText(TextFormat("%d", i+1), brick[i].position.x + brick[i].brickWidth / 2 - 2.5, brick[i].position.y + brick[i].brickHeight / 2 - 5, 5, RED);
#endif // _DEBUG
        }
    }
}
void cBricks::Input() {
    //
}
