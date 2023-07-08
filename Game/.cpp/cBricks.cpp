#include "cBricks.h"

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
void cBricks::Logic() {
    //
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
