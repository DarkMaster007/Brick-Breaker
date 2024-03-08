#include "editor.h"
#include "cstring"
#include "DrawFunctions.h"

extern bool quit;
extern int animationFrame;
extern double updateInterval;
int selectedBrickID;
extern cAnimBall *animationBalls;
cBricks *editorBricks = nullptr;
cBricks *tmpBricks = nullptr;
Rectangle ghostBrick;
float ghostBrickWidth = 50, ghostBrickHeight = 35;
Color ghostBrickColor = WHITE;
int ghostBrickType = 1;
int ballAnimIndex = 0;
int selectedAxis = 0; //0 - X, 1 - Y


void Editor_Init() { // load the brick variable with only ones (meaning all bricks are visible and active)
    selectedBrickID = -1;
    updateInterval = 0.05;
    animationBalls = (cAnimBall *)MemAlloc(MAX_ANIM_BALL_COUNT*sizeof(cAnimBall));
}

void Editor_Info() { //the info menu
    while(1) {
        BeginDrawing();
        ClearBackground(BLACK);
        // The actual message you see on screen from here on:
        DrawText("Level editor for Brick Breaker Raylib Edition",GetScreenWidth() / 2 - 42 * 3, GetScreenHeight() / 7.6,15,WHITE);
        DrawText("Create the model that will appear in the game.",GetScreenWidth() / 2 - 52 * 3, GetScreenHeight() / 2.5,15,WHITE);
        DrawText("Click + ctrl to create a brick. Click it to select it. Right Click to change it's type.",GetScreenWidth() / 2 - 87 * 3, GetScreenHeight() / 2.5 + 35,15,WHITE);
        DrawText("Press \"Save and Quit\" once you are done to quit and save.",GetScreenWidth() / 2 - 59 * 3, GetScreenHeight() / 2.5 + 70,15,WHITE);
        DrawText("Click left mouse button to continue.",GetScreenWidth() / 2 - 36 * 3, GetScreenHeight() - GetScreenHeight() / 7.6,15,WHITE);

        EndDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return;
        }
    }
}

// TODO (Administrator#8#02/10/24): Add the rest of the editor functions
void EditorDraw() {
    BeginDrawing();
    ClearBackground(BLACK);
    if(GetMouseY() < GetScreenHeight() - 350) {
        DrawRectangleRec(ghostBrick, {ghostBrickColor.r, ghostBrickColor.g, ghostBrickColor.b, 100});
    }
    if(editorBricks != nullptr) {
        //Draw Bricks
        for(int i = 0; i < cBricks::brickCount; i++) {
            if(editorBricks[i].getEnabled()) {
                Rectangle brickRec = editorBricks[i].getDimensionsRec();
                switch(editorBricks[i].getType()) {
                case 4:
                    DrawBricksBounce(brickRec, &animationBalls[editorBricks[i].animBallIndex], animationFrame, 1);
                    break;
                case 5:
                    DrawRectangleRec(brickRec, editorBricks[i].getColor());
                    break;
                default:
                    DrawBricksPulse(brickRec, animationFrame, editorBricks[i].getType());
                    break;
                }
#ifdef _DEBUG
                //Draw brick number on bricks if DEBUG:
                DrawText(TextFormat("%d", i+1), brickRec.x + brickRec.width / 2 - 2.5, brickRec.y + brickRec.height / 2 - 5, 5, RED);
#endif // _DEBUG
            } else {
                if(editorBricks[i].getType() == 4) {
                    for(int j = editorBricks[i].animBallIndex; j < editorBricks[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; j++) {
                        if(animationBalls[j].getDirection() != STOP) cAnimBall::Draw(&animationBalls[j]);
                    }
                }
            }
        }
        //
    }
    EndDrawing();
}

void EditorLogic() {
    if(IsKeyPressed(KEY_X))
        selectedAxis = 0;
    if(IsKeyPressed(KEY_Y))
        selectedAxis = 1;
    if(selectedAxis) {
        if(IsKeyDown(KEY_KP_ADD))
            ghostBrickHeight++;
        else if(IsKeyDown(KEY_KP_SUBTRACT))
            ghostBrickHeight--;
    } else {
        if(IsKeyDown(KEY_KP_ADD))
            ghostBrickWidth++;
        else if(IsKeyDown(KEY_KP_SUBTRACT))
            ghostBrickWidth--;
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        ghostBrickType++;
        if(ghostBrickType == 6) ghostBrickType = 1;
    }
    switch(ghostBrickType) {
    case 1:
        ghostBrickColor =  WHITE;
        break;
    case 2:
        ghostBrickColor =  BEIGE;
        break;
    case 3:
        ghostBrickColor =  GRAY;
        break;
    case 4:
        ghostBrickColor =  ORANGE;
        break;
    case 5:
        ghostBrickColor =  GOLD;
        break;
    }

    ghostBrick = {GetMouseX() - ghostBrickWidth / 2, GetMouseY() - ghostBrickHeight / 2, ghostBrickWidth, ghostBrickHeight};
    if(GetMouseY() < GetScreenHeight() - 350 - ghostBrickHeight / 2 && GetMouseY() > 15 + ghostBrickHeight / 2 && GetMouseX() > 15 + ghostBrickWidth / 2 && GetMouseX() < GetScreenWidth() - 15 - ghostBrickWidth / 2 && IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cBricks::brickCount < MAX_BRICKS) {
        bool badCollision = 0;
        for(int i = 0; i < cBricks::brickCount; i++) {
            badCollision = CheckCollisionRecs(ghostBrick, editorBricks[i].getDimensionsRec());
            if(badCollision) {
                break;
            }
        }
        if(!badCollision) {
            tmpBricks = (cBricks*)MemRealloc(editorBricks,sizeof(cBricks) * (cBricks::brickCount + 1));
            if(tmpBricks != nullptr) {
                editorBricks = tmpBricks;
                ballAnimIndex = cAnimBall::ballCount;
                new (&editorBricks[cBricks::brickCount]) cBricks(GetMouseX() - ghostBrickWidth / 2, GetMouseY() - ghostBrickHeight / 2, ghostBrickWidth, ghostBrickHeight, ghostBrickType, ballAnimIndex);
                if(ghostBrickType == 4) {
                    int ballSize = 3;
                    int ballSpeed = 50;
                    for(int j = 0; j < STANDARD_ANIM_BALL_COUNT; j++) {
                        new (&animationBalls[ballAnimIndex + j]) cAnimBall(GetRandomValue(ghostBrick.x + 6, ghostBrick.x + ghostBrickWidth - 6), GetRandomValue(ghostBrick.y + 6, ghostBrick.y + ghostBrickHeight - 6), ballSize, ballSpeed);
                        animationBalls[ballAnimIndex + j].randomDirection();
                    }
                }
            }
        }
    }
    for(int i = 0; i < cAnimBall::ballCount; i++) {
        animationBalls[i].Move();
    }
    //Animation ball collision
    for(int i = 0; i < cBricks::brickCount; i++) {
        if(editorBricks[i].getType() != 4) continue;
        for(int animBallID = editorBricks[i].animBallIndex; animBallID < editorBricks[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; animBallID++) {
            if(animationBalls[animBallID].getY() + animationBalls[animBallID].getSize() > GetScreenHeight() * 0.8) animationBalls[animBallID].setDirection(STOP);
            if(animationBalls[animBallID].getDirection() == STOP) continue;
            for(int j = 0; j < cBricks::brickCount; j++) {
                if(editorBricks[j].getType() == 4 || editorBricks[j].getEnabled() == false) continue;
                if(CheckCollisionCircleRec(animationBalls[animBallID].getV(), animationBalls[animBallID].getSize(), editorBricks[j].getDimensionsRec())) {
                    animationBalls[animBallID].setDirection(STOP);
                    editorBricks[j].callOnCollision();
                }
            }
        }
    }
    if(GuiButton({GetScreenWidth() / 2 - 250, GetScreenHeight() - 160, 200, 80}, "Save and Quit")) {
        EditorOutput();
        quit = 1;
    }
    if(GuiButton({GetScreenWidth() / 2 + 50, GetScreenHeight() - 160, 200, 80}, "Quit")) {
        quit = 1;
    }
}

void EditorCleanup() {
    int totalBrickCount = cBricks::brickCount;
    for(int i = 0; i < totalBrickCount; i++) {
        editorBricks[i].~cBricks();
    }
    int totalBallCount = cAnimBall::ballCount;
    for(int i = 0; i < totalBallCount; i++) {
        animationBalls[i].~cAnimBall();
    }
    MemFree(animationBalls);
    MemFree(editorBricks);
    _fcloseall();
    CloseWindow();
}

int searchString(char *arrayToParseIn, char stringToSearchFor[], char stringToReplaceWith[]) {   //Allows you to replace part of string; not in use yet
    int i = 0;
    int arraySize = strlen(arrayToParseIn);
    int searchSize = strlen(stringToSearchFor);

    char temp_compare[searchSize + 1];
    char temp_arrayhead[arraySize + 1];
    char temp_arraybottom[arraySize + 1];
    memset(temp_compare, 0, searchSize*sizeof(char));
    memset(temp_arrayhead, 0, arraySize*sizeof(char));
    memset(temp_arraybottom, 0, arraySize*sizeof(char));

    while(i < arraySize) {
        if(arrayToParseIn[i] == stringToSearchFor[0]) {
            strncpy(temp_compare, &arrayToParseIn[i], searchSize);
            temp_compare[searchSize] = '\0';
            if(strcmp(temp_compare, stringToSearchFor) == 0) {
                strncpy(temp_arrayhead, arrayToParseIn, i);
                temp_arrayhead[i] = '\0';
                strncpy(temp_arraybottom, &arrayToParseIn[searchSize + i], arraySize - i - searchSize);
                temp_arraybottom[arraySize - i - searchSize] = '\0';
                sprintf(arrayToParseIn, "%s%s%s", temp_arrayhead, stringToReplaceWith, temp_arraybottom);
                return i;
            }
        }
        i++;
    }
    return -1;
}

void EditorOutput() { //output the current brick layout to the level.txt file
    int i;
    char text[255];
    for(i = 0; i < 100; i++) {
        sprintf(text, "..//config//level%i.txt",i);
        if(!FileExists(text)) {
            break;
        }
    }
    if(i == 100) return;
    FILE *fp = fopen(text, "w");
    // output brick layout to level.txt
    int brickTotal = cBricks::brickCount;
    fprintf(fp, "%d ", brickTotal);
    for(int i = 0; i < cBricks::brickCount; i++) {
        fprintf(fp, "%f %f %f %f %d ", editorBricks[i].getX(), editorBricks[i].getY(), editorBricks[i].getDimensions().x, editorBricks[i].getDimensions().y, editorBricks[i].getType());
#ifdef _DEBUG
        printf("\nBrick Position:\n");
        printf("     X: %f\n", editorBricks[i].getX());
        printf("     Y: %f\n", editorBricks[i].getY());
        printf("Brick Width: %d\n", editorBricks[i].getDimensions().x);
        printf("Brick Height: %d\n", editorBricks[i].getDimensions().y);
        printf("Brick type: ");
        switch(editorBricks[i].getType()) {
        case 0:
            printf("Normal\n");
            break;
        case 1:
            printf("1HP\n");
            break;
        case 2:
            printf("2HP\n");
            break;
        case 3:
            printf("Gold(Unbreakable)\n");
            break;
        case 4:
            printf("Explosive\n\n");
            break;
        }
#endif
    }
    //
}
