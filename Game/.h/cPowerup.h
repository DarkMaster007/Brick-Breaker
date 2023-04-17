#pragma once
#ifndef POWERUP_H
#define POWERUP_H

#include "defines.h"

class Powerup
{
    public:
        Powerup();
        ~Powerup();

        Vector2 Getpos() { return pos; }
        void Setpos(Vector2 val) { pos = val; }

    protected:

    private:
        Vector2 pos;
};

#endif // POWERUP_H
