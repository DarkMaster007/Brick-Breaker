#include "cBricks.h"

Color cBricks::getColor(){
    switch(type){
        case 1: return SKYBLUE;
        case 2: return BLUE;
        case 3: return GRAY;
        case 4: return ORANGE;
        case 5: return GOLD;
    }
}
