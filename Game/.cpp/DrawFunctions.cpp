#include "DrawFunctions.h"

extern Texture2D texPowerup[13];
extern int activePowerups;
extern char rootPath[2048];

void LoadPowerupTextures() {
    //|Pierce|+1 Life|Explode|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (12 bits)
    Image tmp;
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_PIERCE));
    ImageResize(&tmp, 50, 50);
    texPowerup[0] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_HP));
    ImageResize(&tmp, 50, 50);
    texPowerup[1] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_EXPLODE));
    ImageResize(&tmp, 50, 50);
    texPowerup[2] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_MAGNET));
    ImageResize(&tmp, 50, 50);
    texPowerup[3] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_DEATH));
    ImageResize(&tmp, 50, 50);
    texPowerup[4] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_SHRINKBALL));
    ImageResize(&tmp, 50, 50);
    texPowerup[5] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_FASTSPEED));
    ImageResize(&tmp, 50, 50);
    texPowerup[6] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_SUPERSHRINKPADDLE));
    ImageResize(&tmp, 50, 50);
    texPowerup[7] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_FALLINGBRICKS));
    ImageResize(&tmp, 50, 50);
    texPowerup[8] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_EXPANDPADDLE));
    ImageResize(&tmp, 50, 50);
    texPowerup[9] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_SHRINKPADDLE));
    ImageResize(&tmp, 50, 50);
    texPowerup[10] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    tmp = LoadImage(TextFormat("%s%s", rootPath, TEX_POWERUP_SPLITBALL));
    ImageResize(&tmp, 50, 50);
    texPowerup[11] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
    texPowerup[12] = LoadTexture(TextFormat("%s%s", rootPath, TEX_POWERUP_MAGNET_PADDLE));
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
    DrawRectangleRoundedLinesEx(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[], Color ballColor) {
    //Calculate x and y
    for(int kk = 0; kk < STANDARD_ANIM_BALL_COUNT; kk++) {
        //DrawCircle
        cAnimBall::Draw(&animationBalls[kk], ballColor);
    }
    //Draw first rectangle
    DrawRectangleRoundedLinesEx(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksUnbreakable(Rectangle brickRec, Color recColor) {
    // Implementation of DrawBricksUnbreakable
    DrawRectangleRec(brickRec, recColor);
    DrawRectangleRoundedLinesEx(brickRec, 0.2, 10, 3, ORANGE);
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
    if(activePowerups & (1 << 3)) {
        Source = {0, 0, (float)texPowerup[12].width, (float)texPowerup[12].height};
        Destination = {paddle.x, paddle.y - texPowerup[12].height, paddle.width, (float)texPowerup[12].height};
        DrawTexturePro(texPowerup[12], Source, Destination, { 0, 0 }, 0, SKYBLUE);
    }
}
