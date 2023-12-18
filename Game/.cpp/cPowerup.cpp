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
        printf("Powerup enabled!");
        setEnabled(1);
        type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
        printf(" Type: %i\n", type);
        x = brick->getX();
        y = brick->getY();
        return 1;
    }
    return 0;
}

void cPowerup::Logic() {
    // Disable out-of-bounds powerups
    if(getPosition().y > GetScreenHeight() - 10) {
        setEnabled(0);
    }
}
void cPowerup::Draw(cPowerup *powerup) {
    for(int i = 0; i < 6; i++) {
        if(powerup[i].getEnabled()) {
            int movement_speed_powerup = GetRandomValue(300, 400) * GetFrameTime();
            DrawTexture(powerup->texPowerup, powerup[i].getPosition().x - 50, powerup[i].getPosition().y - 25, WHITE);
            powerup[i].setPosition(powerup[i].getPosition().x,powerup[i].getPosition().y + movement_speed_powerup);
        }
    }
}
void cPowerup::Input() {
    //
}
