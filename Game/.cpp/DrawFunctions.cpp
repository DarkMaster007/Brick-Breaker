#include "DrawFunctions.h"

void DrawBricksPulse(Rectangle brickRec, int currentAnimationFrame, int pulseAmount) {
    Rectangle rec;
    //Calculate ratios used to make time to shrink similar between width and height
    float calcHoriz = brickRec.height / brickRec.width;
    float calcVert = brickRec.width / brickRec.height;
    float stepX = (brickRec.width * 0.5) / (pulseAmount + 1);
    float stepY = (brickRec.height * 0.5) / (pulseAmount + 1);

    if(calcHoriz <= 0) calcHoriz = 1;
    if(calcVert <= 0) calcVert = 1;
    if(calcVert > calcHoriz) calcHoriz = 1;
    else calcVert = 1;

    float offsetX, offsetY;

    for(int kk = 0; kk < pulseAmount; kk++) {
        offsetX = fmod((stepX * kk + (currentAnimationFrame + kk) / calcHoriz), brickRec.width / 2);
        offsetY = fmod((stepY * kk + (currentAnimationFrame + kk) / calcVert), brickRec.height / 2);

        rec.x = brickRec.x + offsetX;
        rec.y = brickRec.y + offsetY;
        rec.width = brickRec.width - offsetX * 2;
        rec.height = brickRec.height - offsetY * 2;

        DrawRectangleLinesEx(rec, 1.5, ORANGE);
    }

    //Draw first rectangle
    DrawRectangleRoundedLines(brickRec, 0.1, 1000, 3, ORANGE);
}

void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], int currentAnimationFrame, int brickWallThickness) {
    //Calculate x and y
    int limitX = (brickRec.x + brickRec.width - brickWallThickness);
    int limitY = (brickRec.y + brickRec.height - brickWallThickness);
    for(int kk = 0; kk < STANDARD_ANIM_BALL_COUNT; kk++) {
        if(animationBalls[kk].getX() > limitX) {
            if(animationBalls[kk].getDirection() == DOWNRIGHT) {
                animationBalls[kk].setDirection(DOWNLEFT);
            } else animationBalls[kk].setDirection(UPLEFT);
        }
        if(animationBalls[kk].getX() < (brickRec.x + 9)) {
            if(animationBalls[kk].getDirection() == DOWNLEFT) {
                animationBalls[kk].setDirection(DOWNRIGHT);
            } else animationBalls[kk].setDirection(UPRIGHT);
        }
        if(animationBalls[kk].getY() > limitY) {
            if(animationBalls[kk].getDirection() == DOWNRIGHT) {
                animationBalls[kk].setDirection(UPRIGHT);
            } else animationBalls[kk].setDirection(UPLEFT);
        }
        if(animationBalls[kk].getY() < (brickRec.y + 9)) {
            if(animationBalls[kk].getDirection() == UPRIGHT) {
                animationBalls[kk].setDirection(DOWNRIGHT);
            } else animationBalls[kk].setDirection(DOWNLEFT);
        }
        animationBalls[kk].Move();
        //DrawCircle
        cAnimBall::Draw(&animationBalls[kk]);
    }
    //Draw first rectangle
    DrawRectangleRoundedLines(brickRec, 0.1, 1000, 3, ORANGE);
}

void DrawBricksUnbreakable() {
    // Implementation of DrawBricksUnbreakable
}
