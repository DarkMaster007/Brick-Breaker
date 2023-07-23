#include "cPowerup.h"
#include "cBricks.h"

Texture2D cPowerup::texPowerup;

cPowerup::cPowerup() {
    if(texPowerup.id == 0) {
        Image imgPowerup = LoadImage(TEX_POWERUP);
        ImageResize(&imgPowerup, GetScreenWidth() * 0.04, GetScreenHeight() * 0.04);
        texPowerup = LoadTextureFromImage(imgPowerup);
    }
}

bool cPowerup::spawnPowerup(cBricks brick) {
    if(GetRandomValue(0, 99) < 35) {
        printf("Powerup enabled!");
        setEnabled(1);
        type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
        printf(" Type: %i\n", type);
        position = brick.position;
        return 1;
    }
    return 0;
}

void cPowerup::Logic(cPowerup *powerup) {
    // Disable out-of-bounds powerups
    for(int i = 0; i < 6; i++) {
        if(powerup[i].getPosition().y > GetScreenHeight() - 10) {
            powerup[i].setEnabled(0);
        }
    }
}
void cPowerup::Draw(cPowerup *powerup) {
    for(int i = 0; i < 6; i++) {
        if(powerup[i].getEnabled()) {
            int movement_speed_powerup = GetRandomValue(300, 400) * GetFrameTime();
            DrawTexture(powerup->texPowerup, powerup[i].getPosition().x - 50, powerup[i].getPosition().y - 25, WHITE);
            powerup[i].setPosition(Vector2{powerup[i].getPosition().x,powerup[i].getPosition().y + movement_speed_powerup});
        }
    }
}
void cPowerup::Input() {
    //
}
