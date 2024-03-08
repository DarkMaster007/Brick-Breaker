#define RAYGUI_IMPLEMENTATION
#include "defines.h"
#include "DrawFunctions.h"
#include "editor.h"
#include "cBricks.h"
#include "cBall.h"
#include "cPaddle.h"
#include "cPowerup.h"
#include "cSettings.h"

cBricks *brick;
cAnimBall *animationBalls;
cBall *ball;                    // ball object
cPaddle *paddle;                // paddle object
cPowerup *powerup;              // Handles powerups
Texture texBackground;

Rectangle borderLeft;           // left border rectangle, for collision and drawing
Rectangle borderRight;          // right border rectangle, for collision and drawing
Rectangle borderTop;            // top border rectangle, for collision and drawing
Rectangle borderBottom;         // bottom border rectangle, for collision and drawing

Wave waveBounceGeneral;
Wave waveBouncePaddle;
Wave waveDeath;
Wave waveSelect;

Sound soundBouncePaddle;
Sound soundBounceGeneral;
Sound soundDeath;
Sound soundSelect;
float soundVolume;
bool soundMute;

bool fullscreen;                // whether it's full-screen or not
bool quit;                      // whether the game should quit or not
bool autoMove;                  // whether the paddle automatically moves or not. helps with testing simple stuff
bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
bool isPaused;
double timer;
int levelCounter;
double previousTime;
double updateInterval; //50 ms
int animationFrame;
bool configLoadError;

void OpenWindow();
int MainMenu();
void Init();
int LoadLevel();
void SpawnPowerup(int brickIndex);
void Draw();
void Input();
Vector2 BrickCollision();
void Logic();
void reset();
void checkWin();
int Run();
void Unload();

int main() {
    int exitCode = 0;
    OpenWindow();
    int state = MainMenu();
    if(state == 3) {
        //Editor
        Editor_Init();
        Editor_Info();
        while(!WindowShouldClose() && !quit) {
            EditorLogic();
            timer += GetFrameTime();
            double currentTime = GetTime();
            if(currentTime - previousTime >= updateInterval) {
                if(animationFrame >= 100) animationFrame = 0;
                if(!isPaused) {
                    animationFrame++;
                }
                previousTime = currentTime;
            }
            EditorDraw();
        }
        EditorCleanup();
    }
    if(state == 4) {
        //Game
        Init();
        if(LoadLevel() != 0) {
            exitCode = -1;
        };
        while(!WindowShouldClose() && !win && !quit) {
            Input();
            if(!isPaused) Logic();
            checkWin();
            if(ball->getDirection() != STOP && !isPaused) timer += GetFrameTime();
            double currentTime = GetTime();
            if(currentTime - previousTime >= updateInterval) {
                if(animationFrame >= 100) animationFrame = 0;
                if(!isPaused) {
                    animationFrame++;
                }
                previousTime = currentTime;
            }
            Draw();
        }
        //
        Unload();
    }
    return exitCode;
}

void OpenWindow() {
    FILE *fp;
    int width, height;
    char path[255];
    sprintf(path, "%s\\config\\settings.txt", GetPrevDirectoryPath(GetWorkingDirectory()));
    fp = fopen(path, "r"); //Open settings.txt file
    if(fp == nullptr) {
        fclose(fp);
        width = 800, height = 600, fullscreen = 0;
        fp = fopen(path, "w"); //Open settings.txt file
        if(fp != nullptr) {
            fprintf(fp, "%d %d %d ", width, height, fullscreen);
            configLoadError = 0;
        } else configLoadError = 1;
    } else {
        configLoadError = 0;
        fscanf(fp, "%d %d %d", &width, &height, (int*)&fullscreen); //Load the settings file
    }
    fclose(fp);

#ifdef _DEBUG
    printf("Width: %d\nHeight: %d\nFullscreen: %d\n", width, height, fullscreen);
#endif // _DEBUG

    SetRandomSeed(time(NULL)); //Set seed for random values with current time
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(width,height,"RayBreaker");
    InitAudioDevice();
    SetTargetFPS(120); // TODO (Administrator#7#): Make everything use deltaTime so you can remove the FPS limit
    if(fullscreen == 1) {
        ToggleFullscreen();
    }
}

int MainMenu() {
    int currentScreen = 0;

    //Load Textures
    Texture title = LoadTexture("..//resources//title_main.png");
    //Draw
    char msg_error[95] = "settings.txt file failed to load and was recreated. \nPlease Press \'Esc\' to quit and relaunch.";
    int textsize;

    while(!WindowShouldClose()) {
        //Main screen
        int box_width = 180;
        int box_height = 60;
        int x = GetScreenWidth() / 2 - box_width / 2;
        int y = GetScreenHeight() * 0.35;
        Rectangle button_settings = { (float)x, (float)y, (float)box_width, (float)box_height };
        Rectangle button_extra = { (float)x, (float)(y + box_height + 20), (float)box_width, (float)box_height };
        Rectangle button_editor = { (float)x, (float)(y + box_height * 2 + 20 * 2), (float)box_width, (float)box_height };
        Rectangle button_start = { (float)x, (float)(y + box_height + 20 + 200), (float)box_width, (float)box_height };
        //
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(title, GetScreenWidth() /2 - title.width / 2, 80, WHITE); // Draw Settings texture (to change)

        if(!configLoadError) {
            //General Menu
            if(currentScreen == 0) {
                if(GuiButton(button_settings, "Settings")) {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    currentScreen = 1;
                }

                if(GuiButton(button_extra, "Extra")) {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    currentScreen = 2;
                }

                if(GuiButton(button_editor, "Editor")) {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    return 3;
                }


                if(GuiButton(button_start, "Start")) {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    return 4;
                }
            }
            if(currentScreen == 1)
                drawSettings(currentScreen);
        } else {
            textsize = MeasureText(msg_error,25);
            DrawText(msg_error, (float)(GetScreenWidth() / 2 - textsize / 2), (float)(GetScreenHeight() / 2), 25, BLACK);
        }

        EndDrawing();
    }
    return 0;
}

void Init() {
    HideCursor();
    //Initialize all vars with their proper values
    quit = 0;
    autoMove = 0;
    win = 0;
    isPaused = 0;
    levelCounter = 0;
    previousTime = GetTime();
    updateInterval = 0.05; //50 ms
    animationFrame = 0;
    timer = 0;
    //

    ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, 10, 250);
    paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
    animationBalls = (cAnimBall *)MemAlloc(MAX_ANIM_BALL_COUNT*sizeof(cAnimBall));
    brick = (cBricks *)MemAlloc(MAX_BRICKS*sizeof(cBricks));  //MemAlloc() is equivalent to malloc();
    powerup = new cPowerup[6];
    texBackground = LoadTexture("..//resources//background.png");

    //Load sound from .h files
    waveDeath.channels = WAVEDEATH_CHANNELS;
    waveDeath.data = waveDeathData;
    waveDeath.frameCount = WAVEDEATH_FRAME_COUNT;
    waveDeath.sampleRate = WAVEDEATH_SAMPLE_RATE;
    waveDeath.sampleSize = WAVEDEATH_SAMPLE_SIZE;
    soundDeath = LoadSoundFromWave(waveDeath);

    waveSelect.channels = WAVESELECT_CHANNELS;
    waveSelect.data = waveSelectData;
    waveSelect.frameCount = WAVESELECT_FRAME_COUNT;
    waveSelect.sampleRate = WAVESELECT_SAMPLE_RATE;
    waveSelect.sampleSize = WAVESELECT_SAMPLE_SIZE;
    soundSelect = LoadSoundFromWave(waveSelect);

    waveBounceGeneral.channels = WAVEBOUNCEGENERAL_CHANNELS;
    waveBounceGeneral.data = waveBounceGeneralData;
    waveBounceGeneral.frameCount = WAVEBOUNCEGENERAL_FRAME_COUNT;
    waveBounceGeneral.sampleRate = WAVEBOUNCEGENERAL_SAMPLE_RATE;
    waveBounceGeneral.sampleSize = WAVEBOUNCEGENERAL_SAMPLE_SIZE;
    soundBounceGeneral = LoadSoundFromWave(waveBounceGeneral);

    waveBouncePaddle.channels = WAVEBOUNCEPADDLE_CHANNELS;
    waveBouncePaddle.data = waveBouncePaddleData;
    waveBouncePaddle.frameCount = WAVEBOUNCEPADDLE_FRAME_COUNT;
    waveBouncePaddle.sampleRate = WAVEBOUNCEPADDLE_SAMPLE_RATE;
    waveBouncePaddle.sampleSize = WAVEBOUNCEPADDLE_SAMPLE_SIZE;
    soundBouncePaddle = LoadSoundFromWave(waveBouncePaddle);

    SetSoundVolume(soundBouncePaddle, 0.1f);
    SetSoundVolume(soundBounceGeneral, 0.2f);
    SetSoundVolume(soundDeath, 0.5f);
    SetSoundVolume(soundSelect, 1.0f);
    soundVolume = 1.0f;
    soundMute = 0;
    SetMasterVolume(soundVolume);

    //Rectangles that will be used for collision
    float borderThickness = 15;
    borderLeft = {0, borderThickness - 0.3f, borderThickness, (float)GetScreenHeight() - borderThickness};
    borderRight = {(float)(GetScreenWidth() - borderThickness), borderThickness - 0.3f, borderThickness, (float)GetScreenHeight() - borderThickness};
    borderTop = {0, 0, (float)GetScreenWidth(), borderThickness};
    borderBottom = {0, (float)(GetScreenHeight() - borderThickness), (float)GetScreenWidth(), borderThickness};
}

int LoadLevel() {
    FILE *fp;
    int i = 0, brickTotal = 0;
    float brickPosX, brickPosY;
    float brickWidth, brickHeight;
    int brickType;
    Rectangle progressBarRec = {GetScreenWidth() * 0.12f, GetScreenHeight() / 2 - 50.0f, GetScreenWidth() - GetScreenWidth() * 0.24f, 100};

    fp = fopen(TextFormat("..\\config\\level%i.txt", levelCounter), "r");
    if(!fp) {
        if(levelCounter == 0) {
            return -1;
        } else {
            levelCounter--;
            fp = fopen(TextFormat("..\\config\\level%i.txt", levelCounter), "r");
            if(!fp) return -1;
        }
    }
    if(!feof(fp)) {
        fscanf(fp, "%d", &brickTotal);
    }
    while(!feof(fp)) {
        float progress = i;
        BeginDrawing();
        ClearBackground(BLACK);
        GuiProgressBar(progressBarRec, "0", "100", &progress, 0, brickTotal);
        fscanf(fp, "%f %f %f %f %d", &brickPosX, &brickPosY, &brickWidth, &brickHeight, &brickType);
        if(brickPosX > 0 && brickPosX < GetScreenWidth() && brickPosY > 0 && brickPosY < GetScreenHeight()) {
            int ballAnimIndex;
            int ballSize = 3;
            int ballSpeed = 50;
            if(brickType == 4)
                ballAnimIndex = cAnimBall::ballCount;
            else
                ballAnimIndex = -1;
            new (&brick[i]) cBricks(brickPosX, brickPosY, brickWidth, brickHeight, brickType, ballAnimIndex);
            if(brickType == 4) {
                for(int j = 0; j < STANDARD_ANIM_BALL_COUNT; j++) {
                    new (&animationBalls[ballAnimIndex + j]) cAnimBall(GetRandomValue(brickPosX + 6, brickPosX + brickWidth - 6), GetRandomValue(brickPosY + 6, brickPosY + brickHeight - 6), ballSize, ballSpeed);
                    animationBalls[ballAnimIndex + j].randomDirection();
                }
            }
            i++;
        }
        EndDrawing();
    }
    if(cAnimBall::ballCount % STANDARD_ANIM_BALL_COUNT != 0) throw "Invalid animation ball count";
    fclose(fp);
    return 0;
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    //Draw Bricks
    for(int i = 0; i < cBricks::brickCount; i++) {
        if(brick[i].getEnabled()) {
            Rectangle brickRec = brick[i].getDimensionsRec();
            switch(brick[i].getType()) {
            case 4:
                DrawBricksBounce(brickRec, &animationBalls[brick[i].animBallIndex], animationFrame, 1);
                break;
            case 5:
                DrawRectangleRec(brickRec, brick[i].getColor());
                break;
            default:
                DrawBricksPulse(brickRec, animationFrame, brick[i].getType());
                break;
            }
#ifdef _DEBUG
            //Draw brick number on bricks if DEBUG:
            DrawText(TextFormat("%d", i+1), brickRec.x + brickRec.width / 2 - 2.5, brickRec.y + brickRec.height / 2 - 5, 5, RED);
#endif // _DEBUG
        } else {
            if(brick[i].getType() == 4) {
                for(int j = brick[i].animBallIndex; j < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; j++) {
                    if(animationBalls[j].getDirection() != STOP) cAnimBall::Draw(&animationBalls[j]);
                }
            }
        }
    }
    //

    //Draw Paddle
    DrawPaddle(paddle->getDimensionsRec(), paddle->getBounceReverseArea(), paddle->textureEdge_L, paddle->textureEdge_R, paddle->textureBody);
    //

    //Draw Ball
    DrawCircle(ball->getX(), ball->getY(), ball->getSize(), WHITE);
    //

    //Draw Powerups
    for(int i = 0; i < 6; i++) {
        if(powerup[i].getEnabled()) {
            DrawTexture(powerup->texPowerup, powerup[i].getPosition().x - 50, powerup[i].getPosition().y - 25, WHITE);
            powerup[i].setPosition(powerup[i].getPosition().x, powerup[i].getPosition().y + GetRandomValue(300, 400) * GetFrameTime());
        }
    }
    //

    //Draw Borders
    int distance = 0;
    int ballSize;
    // Define a maximum size for the ball and a factor for size decrease
    int maxSize = ball[0].getSize() * 3;
    int minSize = ball[0].getSize() + 4;
    //DRAW BORDER TOP
    DrawRectangleRec(borderTop, BLACK);
    DrawRectangleLinesEx(borderTop, 1.5, BLUE);
    if(CheckCollisionCircleRec({ball[0].getX(),ball[0].getY()}, ball[0].getSize(), borderTop)) {
        distance = ball[0].getY() - borderTop.y - borderTop.height;
        // Calculate the ball size based on the distance
        ballSize = maxSize * (distance / 10.0f);
        // Ensure the ball size does not go below the minimum size
        if (ballSize > minSize) {
            ballSize = minSize;
        }
        BeginScissorMode(borderTop.x, borderTop.y, borderTop.width, borderTop.height);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize, SKYBLUE);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize - 2, BLACK);
        EndScissorMode();
    }
    //
    DrawRectangleRec(borderLeft, BLACK);
    DrawRectangleLinesEx(borderLeft, 1.5, BLUE);
    if(CheckCollisionCircleRec({ball[0].getX(),ball[0].getY()}, ball[0].getSize(), borderLeft)) {
        distance = ball[0].getX() - borderLeft.x - borderLeft.width;
        // Calculate the ball size based on the distance
        ballSize = maxSize * (distance / 10.0f);
        // Ensure the ball size does not go below the minimum size
        if (ballSize > minSize) {
            ballSize = minSize;
        }
        BeginScissorMode(borderLeft.x, borderLeft.y, borderLeft.width, borderLeft.height);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize, SKYBLUE);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize - 2, BLACK);
        EndScissorMode();
    }
    //
    DrawRectangleRec(borderRight, BLACK);
    DrawRectangleLinesEx(borderRight, 1.5, BLUE);
    if(CheckCollisionCircleRec({ball[0].getX(),ball[0].getY()}, ball[0].getSize(), borderRight)) {
        distance = borderRight.x - ball[0].getX();
        // Calculate the ball size based on the distance
        ballSize = maxSize * (distance / 10.0f);
        // Ensure the ball size does not go below the minimum size
        if (ballSize > minSize) {
            ballSize = minSize;
        }
        BeginScissorMode(borderRight.x, borderRight.y, borderRight.width, borderRight.height);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize, SKYBLUE);
        DrawCircle(ball[0].getX(), ball[0].getY(), ballSize - 2, BLACK);
        EndScissorMode();
    }
    //

    //Debug Red Bottom Border:
#ifdef _DEBUG
    DrawRectangleRec(borderBottom,RED);
    DrawFPS(10,10);
    if(ball->getDirection() != STOP)
        GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, TextFormat("%f", timer));
    else
        GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, "0.00000");
#endif // _DEBUG
    EndDrawing();
}

void Input() {
    // Enable auto-moving
#ifdef _DEBUG
    if(IsKeyPressed('W')) {
        autoMove = !autoMove;
    }
#endif // _DEBUG

    // Reset the level
    if(IsKeyPressed('R')) {
        reset();
    }

    //Pause the game
    if(IsKeyPressed('P')) {
        isPaused = !isPaused;
    }

    if(IsKeyPressed('F')) {
        ToggleFullscreen();
    }

    if(IsKeyPressed('M')) {
        soundMute = !soundMute;
        SetMasterVolume(soundVolume * !soundMute);
    }

    paddle->Input(autoMove, isPaused);

    //Lock mouse position so it doesn't go outside of screen
    if(!isPaused) {
        SetMousePosition(GetScreenWidth() / 2,GetScreenHeight() / 2);
    }
}

void Logic() {
    ball->Logic(timer);

    paddle->Logic(ball, autoMove, soundBouncePaddle);

    for(int i = 0; i < cBricks::brickCount; i++) {
        brick[i].Logic(ball, powerup, soundBounceGeneral);
    }
    for(int i = 0; i < cAnimBall::ballCount; i++) {
        animationBalls[i].Move();
    }

    for(int i = 0; i < MAX_POWERUPS; i++) {
        if(powerup[i].getPosition().y > GetScreenHeight() - 10) {
            powerup[i].setEnabled(0);
        }
        if(CheckCollisionRecs(paddle->getDimensionsRec(),powerup[i].getDimensionsRec())) {
            powerup[i].setEnabled(0);
        }
    }

    //Animation ball collision
    for(int i = 0; i < cBricks::brickCount; i++) {
        if(brick[i].getType() != 4) continue;
        if(brick[i].getEnabled()) continue;
        for(int animBallID = brick[i].animBallIndex; animBallID < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; animBallID++) {
            if(animationBalls[animBallID].getY() + animationBalls[animBallID].getSize() > GetScreenHeight() * 0.8) animationBalls[animBallID].setDirection(STOP);
            if(animationBalls[animBallID].getDirection() == STOP) continue;
            for(int j = 0; j < cBricks::brickCount; j++) {
                if(brick[j].getEnabled() == false) continue;
                if(CheckCollisionCircleRec(animationBalls[animBallID].getV(), animationBalls[animBallID].getSize(), brick[j].getDimensionsRec())) {
                    animationBalls[animBallID].setDirection(STOP);
                    brick[j].callOnCollision();
                    for(int k = 0; k < MAX_POWERUPS; k++) {
                        if(!powerup[k].getEnabled()) {
                            powerup[k].spawnPowerup(&brick[k]);
                            break;
                        }
                    }
                }
            }
        }
    }

    //Make bricks fall after 60 seconds
    if(ball->getDirection() != STOP && !isPaused) {
        if(timer > 60.0f) {
            if(fmod(timer, 10.0) < 0.10) {
                for(int i = 0; i < cBricks::brickCount; i++) {
                    brick[i].setY(brick[i].getY() + 5);
                }
            }
        }
    }

    //Left wall collision
    Vector2 ball_collision = {ball->getX(), ball->getY()};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(), borderLeft)) {
        if(ball->getDirection() == UPLEFT) {
            ball->setDirection(UPRIGHT);
        }
        if(ball->getDirection() == DOWNLEFT) {
            ball->setDirection(DOWNRIGHT);
        }
        PlaySound(soundBounceGeneral);
    }

    //Right wall collision
    ball_collision = {ball->getX(), ball->getY()};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderRight)) {
        if(ball->getDirection() == UPRIGHT) {
            ball->setDirection(UPLEFT);
        }
        if(ball->getDirection() == DOWNRIGHT) {
            ball->setDirection(DOWNLEFT);
        }
        PlaySound(soundBounceGeneral);
    }

    //Top wall collision
    ball_collision = {ball->getX(), ball->getY()};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderTop)) {
        if(ball->getDirection() == UPRIGHT) {
            ball->setDirection(DOWNRIGHT);
        }
        if(ball->getDirection() == UPLEFT) {
            ball->setDirection(DOWNLEFT);
        }
        PlaySound(soundBounceGeneral);
    }

    //Bottom wall collision
    ball_collision = {ball->getX(), ball->getY()};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom)) {
        PlaySound(soundDeath);
        reset();
    }
}

void reset() {
    timer = 0;
    int totalBrickCount = cBricks::brickCount;
    for(int i = 0; i < totalBrickCount; i++) {
        brick[i].~cBricks();
    }
    int totalBallCount = cAnimBall::ballCount;
    for(int i = 0; i < totalBallCount; i++) {
        animationBalls[i].~cAnimBall();
    }
    paddle->Reset();
    ball->Reset();
    LoadLevel();
    for(int i = 0; i < 6; i++) {
        powerup[i].setEnabled(0);
    }
    SetRandomSeed(time(NULL));
}

void checkWin() {
    int brickAmount = 0;
    for(int i = 0; i <= cBricks::brickCount; i++) {
        if(brick[i].getType() != 5) {
            brickAmount = brickAmount + brick[i].getEnabled();
        }
    }
    if(brickAmount == 0) {
        levelCounter++;
        reset();
    }
}

void Unload() {
    int totalBrickCount = cBricks::brickCount;
    for(int i = 0; i < totalBrickCount; i++) {
        brick[i].~cBricks();
    }
    int totalBallCount = cAnimBall::ballCount;
    for(int i = 0; i < totalBallCount; i++) {
        animationBalls[i].~cAnimBall();
    }
    MemFree(animationBalls);
    MemFree(brick);
    delete[] powerup;
    delete ball;
    delete paddle;
#ifdef _DEBUG
    printf("Destructor finished.\n");
#endif // _DEBUG
    _fcloseall();
    CloseWindow();
}
