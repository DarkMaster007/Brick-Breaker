#include "cBricks.h"

Texture2D cBricks::texture;

cBricks::cBricks(int loadedX, int loadedY, int loadedWidth, int loadedHeight, int loadedType, Texture2D tex){
    position.x = loadedX;
    position.y = loadedY;
    brickWidth = loadedWidth;
    brickHeight = loadedHeight;
    type = loadedType;
    enabled = true;
    if(texture.id == 0) texture = tex;
}

Color cBricks::getColor(){
    switch(type){
        case 1: return SKYBLUE;
        case 2: return BLUE;
        case 3: return GRAY;
        case 4: return ORANGE;
        case 5: return GOLD;
    }
}
