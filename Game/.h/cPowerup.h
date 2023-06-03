#pragma once
#ifndef CPOWERUP_H
#define CPOWERUP_H
#endif // CPOWERUP_H

#include "defines.h"
#include "cBricks.h"

class cPowerup{
    eActivePowerups type;
    Vector2 position;
    bool enabled;
    int spawnChance{35};
public:
    eActivePowerups getType(){ return type;}
    void setType(eActivePowerups newType){ type = newType; }
    Vector2 getPosition(){ return position; }
    void setPosition(Vector2 newPosition){ position = newPosition; }
    bool getEnabled(){ return enabled; }
    void setEnabled(bool newEnabled){ enabled = newEnabled; }
    bool spawnPowerup(cBricks brick);
};
