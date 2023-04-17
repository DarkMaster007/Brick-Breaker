#define RAYGUI_IMPLEMENTATION
#include "defines.h"
#include "cGameManager.h"

int main() {
    cGameManager cGame;
    int exit = cGame.Run();
    return exit;
}
