#include "cPowerup.h"
#include "cBricks.h"

extern int frame;
extern int activePowerups;

RenderTexture2D texPowerup[13];
int cPowerup::powerupCount = 0;

cPowerup::cPowerup() {
    ID = powerupCount;
    powerupCount++;
    spawnChance = 35;
}

bool cPowerup::spawnPowerup(cBricks *brick) {
    if(GetRandomValue(0, 99) < spawnChance) {
        setEnabled(1);
        type = GetRandomValue(0, 12);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
        rec.x = brick->getX();
        rec.y = brick->getY();
#ifdef _DEBUG
        printf("Powerup enabled!");
        printf(" Type: %i\n", type);
#endif // _DEBUG
        return 1;
    }
    return 0;
}
void cPowerup::triggerEffect() {
    //|Pierce|+1 Life|Explode|FireBall|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (13 bits)
    activePowerups = activePowerups | 1 << type;
    if(type == 0) {
        printf("Pierce enabled.\n");
    }
}
