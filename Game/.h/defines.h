#pragma once

#ifndef DEFINES_H
#define DEFINES_H
#include <raylib.h>
#include <raygui.h>
#include <stdio.h>
#include "soundDeath.h"
#include "soundSelect.h"
#include "soundBounceGeneral.h"
#include "soundBouncePaddle.h"
#include <math.h>
#include <time.h>
#include <random>
#include <chrono>

#define MAX_BRICKS 300
#define MAX_BALLS 10
#define MAX_POWERUPS 6
#define MAX_ANIM_BALL_COUNT 200
#define STANDARD_ANIM_BALL_COUNT 8

#define TEX_BALL "..//resources//ball.png"
#define TEX_PADDLE_BODY "..//resources//Breakout-Paddle_middle.png"
#define TEX_PADDLE_EDGE_L "..//resources//Breakout-Paddle_L.png"
#define TEX_PADDLE_EDGE_R "..//resources//Breakout-Paddle_R.png"
#define TEX_POWERUP "..//resources//powerup_placeholder.png"

enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};
enum eActivePowerups { LASER = 0, CROSSHAIR = 1, EXTEND = 2, PIERCE = 3, FIREBALL = 4, NEXTLEVEL = 5 };

#endif // DEFINES_H
