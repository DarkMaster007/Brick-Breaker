#include "cPowerup.h"
#include "cBricks.h"

Texture2D cPowerup::texPowerup;
int cPowerup::powerupCount = 0;
extern int frame;

cPowerup::cPowerup() {
    ID = powerupCount;
    powerupCount++;
    spawnChance = 35;
    if(texPowerup.id == 0) {
        Image imgPowerup = LoadImage(TEX_POWERUP);
        ImageResize(&imgPowerup, GetScreenWidth() * 0.04, GetScreenHeight() * 0.04);
        texPowerup = LoadTextureFromImage(imgPowerup);
    }
}

bool cPowerup::spawnPowerup(cBricks *brick) {
    if(GetRandomValue(0, 99) < spawnChance) {
        setEnabled(1);
        type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
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
