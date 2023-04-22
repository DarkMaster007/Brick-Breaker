#pragma once
#ifndef CGAMEMANAGER_H
#define CGAMEMANAGER_H
#endif // CGAMEMANAGER_H
#include "defines.h"
#include "cBall.h"
#include "cBricks.h"
#include "cPaddle.h"
#include "cPowerup.h"
#include "cCollisionManager.h"

class cGameManager {
private:
    FILE *fp;
    Bricks *brick;
    Image imgBrick;
    Texture2D texBrick;
    Texture2D texBall;
    Texture2D texPaddleEdge_L;
    Texture2D texPaddleEdge_R;
    Texture2D texPaddleBody;
    Texture2D texPowerup;
    cBall *ball;                    // ball object
    cPaddle *paddle;                // paddle object
    Rectangle borderLeft;           // left border rectangle, for collision and drawing
    Rectangle borderRight;          // right border rectangle, for collision and drawing
    Rectangle borderTop;            // top border rectangle, for collision and drawing
    Rectangle borderBottom;         // bottom border rectangle, for collision and drawing
    powerup *c_powerup;              // Handles powerups

    Wave waveBounceGeneral;
    Wave waveBouncePaddle;
    Wave waveDeath;
    Wave waveSelect;

    Sound soundBouncePaddle;
    Sound soundBounceGeneral;
    Sound soundDeath;
    Sound soundSelect;
    float soundVolume;
    bool soundMute;

    int width,height;               // Screen width and height
    bool fullscreen;                // whether it's full-screen or not
    bool quit;                      // whether the game should quit or not
    int brickCount;
    float movement_speed_paddle_base; // base speed for paddle
    float movement_speed_ball_base;   // base speed for ball
    bool auto_move;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool pause;

public:
    cGameManager();
    void loadLevel();
    void SpawnPowerup(int brickIndex);
    void Draw();
    void Input();
    Vector2 BrickCollision();
    void Logic();
    void reset();
    void checkWin();
    void cleanup();
    int Run();
};
