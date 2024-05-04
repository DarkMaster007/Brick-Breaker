#include "DrawFunctions.h"

extern Texture2D texPowerup[14];
extern int activePowerups;

void GeneratePowerupTextures() {
    //|Pierce|+1 Life|Explode|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (12 bits)
    Image tmp;
    tmp = LoadImage(TEX_POWERUP_PIERCE);
    ImageResize(&tmp, 50, 50);
    texPowerup[0] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TEX_POWERUP_HP);
    ImageResize(&tmp, 50, 50);
    texPowerup[1] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TEX_POWERUP_MAGNET);
    ImageResize(&tmp, 50, 50);
    texPowerup[2] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    texPowerup[13] = LoadTexture(TEX_POWERUP_MAGNET_PADDLE);
    for(int i = 0; i < 13; i++) {
        if(texPowerup[i].id == 0) {
            tmp = LoadImage(TEX_POWERUP);
            ImageResize(&tmp, 50, 50);
            texPowerup[i] = LoadTextureFromImage(tmp);
            UnloadImage(tmp);
        }
    }
}

void DrawBricksPulse(Rectangle brickRec, int currentAnimationFrame, int pulseAmount, Color recColor) {
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

        DrawRectangleLinesEx(rec, 1.5, recColor);
    }

    //Draw first rectangle
    DrawRectangleRoundedLines(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], Color ballColor) {
    //Calculate x and y
    for(int kk = 0; kk < STANDARD_ANIM_BALL_COUNT; kk++) {
        //DrawCircle
        cAnimBall::Draw(&animationBalls[kk], ballColor);
    }
    //Draw first rectangle
    DrawRectangleRoundedLines(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksUnbreakable(Rectangle brickRec, Color recColor) {
    // Implementation of DrawBricksUnbreakable
    DrawRectangleRec(brickRec, recColor);
    DrawRectangleRoundedLines(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawPaddle(Rectangle paddle, float reverseAreaSize, Texture2D edgeTextureL, Texture2D edgeTextureR, Texture2D bodyTexture) {
    Rectangle Source = {0, 0, (float)edgeTextureL.width, (float)edgeTextureL.height};
    Rectangle Destination = {paddle.x, paddle.y, reverseAreaSize, paddle.height};
    DrawTexturePro(edgeTextureL, Source, Destination, { 0, 0 }, 0, WHITE);

    Source = {0, 0, (float)edgeTextureR.width, (float)edgeTextureR.height};
    Destination = {paddle.x + paddle.width - reverseAreaSize, paddle.y, reverseAreaSize, paddle.height};
    DrawTexturePro(edgeTextureR, Source, Destination, { 0, 0 }, 0, WHITE);

    Source = {0, 0, (float)bodyTexture.width, (float)bodyTexture.height};
    Destination = {paddle.x + reverseAreaSize, paddle.y, paddle.width - 2 * reverseAreaSize, paddle.height};
    DrawTexturePro(bodyTexture, Source, Destination, { 0, 0 }, 0, WHITE);
    if(activePowerups & (1 << 3)){
        Source = {0, 0, texPowerup[13].width, texPowerup[13].height};
        Destination = {paddle.x, paddle.y - texPowerup[13].height, paddle.width, texPowerup[13].height};
        DrawTexturePro(texPowerup[13], Source, Destination, { 0, 0 }, 0, SKYBLUE);
    }
}
