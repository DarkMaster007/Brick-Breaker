#include "defines.h"
#include "cBall.h"
#include "cPaddle.h"
#include "cBricks.h"

class cCollisionManager{
public:
    static bool WallCollision(float wallThickness);
    static bool CheckCollision(cBricks &brick, cBall &ball);
    static bool CheckCollision(cPaddle &paddle, cBall &ball);
};
