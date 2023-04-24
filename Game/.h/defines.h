#pragma once
#ifndef DEFINES_H
#define DEFINES_H
#endif // DEFINES_H
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

enum eDir{ STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};
enum eActivePowerups{ LASER = 0, CROSSHAIR = 1, EXTEND = 2, PIERCE = 3, FIREBALL = 4, NEXTLEVEL = 5 };
