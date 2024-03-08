#ifndef CPOWERUP_H
#define CPOWERUP_H

#include "defines.h"

class cBricks;

class cPowerup {
    eActivePowerups type;
    int ID;
    Rectangle rec;
    bool enabled;
    int spawnChance;
  public:
    static int powerupCount;
    static Texture2D texPowerup;

    cPowerup();
    eActivePowerups getType() const{
        return type;
    }
    void setType(eActivePowerups newType) {
        type = newType;
    }
    Vector2 getPosition() {
        return {rec.x, rec.y};
    }
    void setPosition(float newX, float newY) {
        rec.x = newX;
        rec.y = newY;
    }
    void setPosition(Vector2 newPosition) {
        rec.x = newPosition.x;
        rec.y = newPosition.y;
    }
    bool getEnabled() const{
        return enabled;
    }
    void setEnabled(bool newEnabled) {
        enabled = newEnabled;
    }
    bool spawnPowerup(cBricks *brick);
    inline float getX() const;
    inline float getY() const;
    inline Vector2 getDimensions() const;
    inline Rectangle getDimensionsRec() const;
    inline void setX(float);
    inline void setY(float);
    inline void setDimensions(float, float);
    inline void setDimensionsRec(Rectangle);
};

float cPowerup::getX() const {
    return rec.x;
}
float cPowerup::getY() const {
    return rec.y;
}
Vector2 cPowerup::getDimensions() const {
    return {rec.width, rec.height};
}
Rectangle cPowerup::getDimensionsRec() const {
    return rec;
}
void cPowerup::setX(float newX) {
    rec.x = newX;
}
void cPowerup::setY(float newY) {
    rec.y = newY;
}
void cPowerup::setDimensions(float newWidth, float newHeight) {
    rec.width = newWidth;
    rec.height = newHeight;
}
void cPowerup::setDimensionsRec(Rectangle newRectangle) {
    rec = newRectangle;
}

#endif // CPOWERUP_H
