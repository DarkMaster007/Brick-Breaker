#ifndef DRAWFUNCTIONS_H
#define DRAWFUNCTIONS_H

#include "defines.h"
#include "cAnimBall.h"

void LoadPowerupTextures();
void DrawBricksPulse(Rectangle brickRec, int currentAnimationFrame, int lineAmount, Color recColor);
void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], Color ballColor);
void DrawBricksUnbreakable(Rectangle brickRec, Color recColor);

void DrawPaddle(Rectangle paddle, float reverseAreaSize, Texture2D edgeTextureL, Texture2D edgeTextureR, Texture2D bodyTexture);

#endif // DRAWFUNCTIONS_H
