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

    cBricks *brick;
    cBall *ball;                    // ball object
    cPaddle *paddle;                // paddle object
    cPowerup *powerup;              // Handles powerups

    Rectangle borderLeft;           // left border rectangle, for collision and drawing
    Rectangle borderRight;          // right border rectangle, for collision and drawing
    Rectangle borderTop;            // top border rectangle, for collision and drawing
    Rectangle borderBottom;         // bottom border rectangle, for collision and drawing

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

    bool fullscreen;                // whether it's full-screen or not
    bool quit;                      // whether the game should quit or not
    bool autoMove;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool isPaused;
    static double startTimer;

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
    int Run();
    ~cGameManager();
};
