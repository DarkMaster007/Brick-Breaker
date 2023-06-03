#include "cPowerup.h"

bool cPowerup::spawnPowerup(cBricks brick)
{
    if(GetRandomValue(0, 99) < 35)
    {
        printf("Powerup enabled!");
        setEnabled(1);
        type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
        printf(" Type: %i\n", type);
        position = brick.position;
        return 1;
    }
    return 0;
}
