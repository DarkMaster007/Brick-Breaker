#include "cBricks.h"

int cBricks::instanceCount = 0;

cBricks::cBricks()
{
    instanceCount++;
    position = Vector2{ 0, 0 };
    brickWidth = 0;
    brickHeight = 0;
    type = 0;
    enabled = 0;
}

cBricks::~cBricks(){
    instanceCount--;
    printf("Brick deleted");
}
