#ifndef DEFINES_H
#define DEFINES_H

#include <raylib.h>
#include <raygui.h>
#include <stdio.h>
#include "soundDeath.h"
#include "soundSelect.h"
#include "soundBounceGeneral.h"
#include "soundBouncePaddle.h"
#include "soundBounceMagnet.h"
#include <math.h>
#include <time.h>
#include <random>
#include <chrono>

#define MAX_BRICKS 300
#define MAX_BALLS 10
#define MAX_POWERUPS 4
#define STANDARD_ANIM_BALL_COUNT 20
#define MAX_ANIM_BALL_COUNT STANDARD_ANIM_BALL_COUNT * MAX_BRICKS

//Add these to rootPath variable like so: rootPath+TEX_SOMETHING
#define TEX_TITLE_SCREEN "\\Release\\resources\\title_main.png"
#define TEX_PADDLE_BODY "\\Release\\resources\\Breakout-Paddle_middle.png"
#define TEX_PADDLE_EDGE_L "\\Release\\resources\\Breakout-Paddle_L.png"
#define TEX_PADDLE_EDGE_R "\\Release\\resources\\Breakout-Paddle_R.png"
#define TEX_POWERUP "\\Release\\resources\\powerup_placeholder.png"
#define TEX_POWERUP_PIERCE "\\Release\\resources\\textures\\pierce.png"
#define TEX_POWERUP_HP "\\Release\\resources\\textures\\+1.png"
#define TEX_POWERUP_EXPLODE "\\Release\\resources\\textures\\explode.png"
#define TEX_POWERUP_MAGNET_PADDLE "\\Release\\resources\\textures\\magnetPaddle.png"
#define TEX_POWERUP_MAGNET "\\Release\\resources\\textures\\magnet.png"
#define TEX_POWERUP_DEATH "\\Release\\resources\\textures\\death.png"
#define TEX_POWERUP_SHRINKBALL "\\Release\\resources\\textures\\shrinkBall.png"
#define TEX_POWERUP_FASTSPEED "\\Release\\resources\\textures\\fastSpeed.png"
#define TEX_POWERUP_SUPERSHRINKPADDLE "\\Release\\resources\\textures\\superShrinkPaddle.png"
#define TEX_POWERUP_FALLINGBRICKS "\\Release\\resources\\textures\\fallingBricks.png"//
#define TEX_POWERUP_EXPANDPADDLE "\\Release\\resources\\textures\\expandPaddle.png"
#define TEX_POWERUP_SHRINKPADDLE "\\Release\\resources\\textures\\shrinkPaddle.png"
#define TEX_POWERUP_SPLITBALL "\\Release\\resources\\textures\\splitBall.png"

enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};

#endif // DEFINES_H
