#include "DrawFunctions.h"

extern RenderTexture2D texPowerup[13];

void GeneratePowerupTextures() {
    //|Pierce|+1 Life|Explode|FireBall|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (13 bits)
    float powerupSize = 20.0f;
    float sqrt2 = 1.4142f;
    Rectangle textureRec = {(powerupSize * sqrt2) / 2, (powerupSize * sqrt2) / 2, powerupSize * sqrt2, powerupSize * sqrt2};
    texPowerup[0] = LoadRenderTexture(powerupSize * 2, powerupSize * 2);
    BeginTextureMode(texPowerup[0]);
    DrawCircle(texPowerup[0].texture.width / 2, texPowerup[0].texture.height / 2, powerupSize, (Color) {
        124, 221, 255, 200
    });
    DrawRectangle(textureRec.width / 4, textureRec.height / 2 - powerupSize / 4, textureRec.width / 2, -powerupSize / 2, DARKGRAY);
    //DrawTriangle
    EndTextureMode();
}

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
    DrawRectangleRoundedLines(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksBounce(Rectangle brickRec, cAnimBall animationBalls[]) {
    //Calculate x and y
    for(int kk = 0; kk < STANDARD_ANIM_BALL_COUNT; kk++) {
        //DrawCircle
        cAnimBall::Draw(&animationBalls[kk]);
    }
    //Draw first rectangle
    DrawRectangleRoundedLines(brickRec, 0.2, 10, 3, ORANGE);
}

void DrawBricksUnbreakable() {
    // Implementation of DrawBricksUnbreakable
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
}
