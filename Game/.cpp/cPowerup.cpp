#include "cPowerup.h"
#include "cBricks.h"

extern int frame;
extern int activePowerups;

Texture2D texPowerup[13];
int cPowerup::powerupCount = 0;

cPowerup::cPowerup() {
    ID = powerupCount;
    powerupCount++;
    spawnChance = 35;
    enabled = 0;
    type = -1;
}

bool cPowerup::spawnPowerup(cBricks *brick) {
    if(GetRandomValue(0, 99) < spawnChance) {
        setEnabled(1);
        type = GetRandomValue(0, 12);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
        rec.x = brick->getX();
        rec.y = brick->getY();
#ifdef _DEBUG
        printf("Powerup enabled! X: %f, Y: %f\n", rec.x, rec.y);
        printf("Type: %i\n", type);
#endif // _DEBUG
        return 1;
    }
    return 0;
}
void cPowerup::triggerEffect() {
    //|Pierce|+1 Life|Explode|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (12 bits)
    activePowerups = activePowerups | 1 << type;
    if(type == 0) {
        printf("Pierce enabled.\n");
    }
}
