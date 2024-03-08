#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

#include "defines.h"
#include "cAnimBall.h"

void DrawBricksPulse(Rectangle brickRec, int currentAnimationFrame, int lineAmount);
void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], int currentAnimationFrame, int brickWallThickness);
void DrawBricksUnbreakable();

void DrawPaddle(Rectangle paddle, float reverseAreaSize, Texture2D edgeTextureL, Texture2D edgeTextureR, Texture2D bodyTexture);

#endif // DRAWFUNCTIONS_H
