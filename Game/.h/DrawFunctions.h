#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

#include "defines.h"
#include "cAnimBall.h"

void DrawBricksPulse(Rectangle brickRec, int currentAnimationFrame, int lineAmount);
void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], int currentAnimationFrame, int brickWallThickness);
void DrawBricksUnbreakable();

#endif // DRAWFUNCTIONS_H
