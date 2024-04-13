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
int activePowerups;             //|Pierce|+1 Life|Explode|Magnet|Death|ShrinkBall|FastBall|SuperShrinkPaddle|FallingBricks|ExpandPaddle|ShrinkPaddle|SplitBall| (12 bits)

Rectangle borderLeft;           // left border rectangle, for collision and drawing
Rectangle borderRight;          // right border rectangle, for collision and drawing
Rectangle borderTop;            // top border rectangle, for collision and drawing
Rectangle borderBottom;         // bottom border rectangle, for collision and drawing

Wave waveBounceGeneral;
Wave waveBounceMagnet;
Wave waveBouncePaddle;
Wave waveDeath;
Wave waveSelect;

Sound soundBouncePaddle;
Sound soundBounceGeneral;
Sound soundBounceMagnet;
Sound soundDeath;
Sound soundSelect;
float soundVolume;
bool soundMute;

float magnetPowerupDiff; //For magnet powerup. tells how much ball should move with paddle

extern Texture2D texPowerup[13];

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
int lives = 3;

void OpenWindow();
int MainMenu();
void Init();
int LoadLevel();
void Draw();
void Input();
Vector2 BrickCollision();
void Logic();
void gameReset();
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
    int newScreenWidth, newScreenHeight;
    newScreenWidth = GetScreenWidth();
    newScreenHeight = GetScreenHeight();

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
                drawSettings(currentScreen, newScreenWidth, newScreenHeight);
        } else {
            textsize = MeasureText(msg_error,25);
            DrawText(msg_error, (float)(GetScreenWidth() / 2 - textsize / 2), (float)(GetScreenHeight() / 2), 25, BLACK);
        }

        EndDrawing();
    }
    return 0;
}

void Init() {
    GeneratePowerupTextures();
    HideCursor();
    //Initialize all vars with their proper values
    activePowerups = (1 << 3);
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

    waveBounceMagnet.channels = WAVEBOUNCEMAGNET_CHANNELS;
    waveBounceMagnet.data = WAVEBOUNCEMAGNET_DATA;
    waveBounceMagnet.frameCount = WAVEBOUNCEMAGNET_FRAME_COUNT;
    waveBounceMagnet.sampleRate = WAVEBOUNCEMAGNET_SAMPLE_RATE;
    waveBounceMagnet.sampleSize = WAVEBOUNCEMAGNET_SAMPLE_SIZE;
    soundBounceMagnet = LoadSoundFromWave(waveBounceMagnet);
    SetSoundVolume(soundBounceMagnet, 0.5);

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
                DrawBricksBounce(brickRec, &animationBalls[brick[i].animBallIndex], {255,237,191,240});
                break;
            case 5:
                DrawBricksUnbreakable(brickRec, brick[i].getColor());
                break;
            default:
                DrawBricksPulse(brickRec, animationFrame, brick[i].getType(), brick[i].getColor());
                break;
            }
#ifdef _DEBUG
            //Draw brick number on bricks if DEBUG:
            DrawText(TextFormat("%d", i+1), brickRec.x + brickRec.width / 2 - 2.5, brickRec.y + brickRec.height / 2 - 5, 5, RED);
#endif // _DEBUG
        } else {
            if(brick[i].getType() == 4) {
                for(int j = brick[i].animBallIndex; j < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; j++) {
                    if(animationBalls[j].getDirection() != STOP) cAnimBall::Draw(&animationBalls[j], {255,237,191,240});
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
            DrawTexture(texPowerup[powerup[i].getType()], powerup[i].getPosition().x - 50, powerup[i].getPosition().y - 25, WHITE);
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
    GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, TextFormat("%f", timer));
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
        gameReset();
    }

    //Trigger dev stuff
#ifdef _DEBUG
    if(IsKeyPressed(KEY_SPACE)) {
        activePowerups = activePowerups | (1 << 3);
    }
#endif // _DEBUG

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

    if(GetMouseX() > 10 && GetMouseX() < GetScreenWidth() - 10){
        float oldPaddleX = paddle[0].getX();
        paddle->Input(autoMove, isPaused);
        magnetPowerupDiff = paddle[0].getX() -  oldPaddleX; //For magnet powerup to update ball pos
    }
}

void Logic() {
    //Update title
    char title[256];
    sprintf(title, "RayBreaker");
    if(activePowerups & (1 << 0)) { //Pierce
        sprintf(title, "%s|Pierce", title);
    }
    if(activePowerups & (1 << 2)) { //Explode
        sprintf(title, "%s|Explode", title);
    }
    if(activePowerups & (1 << 3)) { //Magnet
        sprintf(title, "%s|Magnet", title);
    }
    if(activePowerups & (1 << 5)) { //ShrinkBall
        sprintf(title, "%s|ShrinkBall", title);
    }
    if(activePowerups & (1 << 8)) { //FallingBricks
        sprintf(title, "%s|FallingBricks", title);
    }
    SetWindowTitle(title);

    // Click to start the ball movement thing
    if(ball[0].getDirection() == STOP) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(timer == 0.0f){
                activePowerups = 0;
            }
            if(ball[0].getX() < paddle[0].getX() + paddle[0].getDimensions().x / 2) {
                ball[0].setDirection(UPLEFT);
            } else
                ball[0].setDirection(UPRIGHT);
        } else {
            if(timer == 0.0f)
                ball[0].setX(GetMouseX() + paddle[0].getDimensions().x / 2);
        }
    }
    ball[0].Move();

    //Paddle
    if(activePowerups & (1 << 3)){
       paddle->Logic(ball, autoMove, soundBounceMagnet);
    }
    else
    paddle->Logic(ball, autoMove, soundBouncePaddle);

    for(int i = 0; i < cAnimBall::ballCount; i++) {
        if(animationBalls[i].getX() < 0 || animationBalls[i].getX() > GetScreenWidth() - borderRight.width || animationBalls[i].getY() < 0 || animationBalls[i].getY() > GetScreenHeight() * 0.8) {
            animationBalls[i].setDirection(STOP);
        }
    }
    for(int i = 0; i < cBricks::brickCount; i++) {
        //Brick logic
        brick[i].Logic(ball, powerup, soundBounceGeneral);
        //Animation ball collision
        int brickEnabled = brick[i].getEnabled();
        Rectangle brickRec = brick[i].getDimensionsRec();
        int limitX = (brickRec.x + brickRec.width - 1);
        int limitY = (brickRec.y + brickRec.height - 1);
        if(brick[i].getType() != 4) continue;
        if(brickEnabled) {
            for(int index = brick[i].animBallIndex; index < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; index++) {
                if(animationBalls[index].getX() > limitX) {
                    if(animationBalls[index].getDirection() == DOWNRIGHT) {
                        animationBalls[index].setDirection(DOWNLEFT);
                    } else animationBalls[index].setDirection(UPLEFT);
                    continue;
                }
                if(animationBalls[index].getX() < (brickRec.x + 9)) {
                    if(animationBalls[index].getDirection() == DOWNLEFT) {
                        animationBalls[index].setDirection(DOWNRIGHT);
                    } else animationBalls[index].setDirection(UPRIGHT);
                    continue;
                }
                if(animationBalls[index].getY() > limitY) {
                    if(animationBalls[index].getDirection() == DOWNRIGHT) {
                        animationBalls[index].setDirection(UPRIGHT);
                    } else animationBalls[index].setDirection(UPLEFT);
                    continue;
                }
                if(animationBalls[index].getY() < (brickRec.y + 9)) {
                    if(animationBalls[index].getDirection() == UPRIGHT) {
                        animationBalls[index].setDirection(DOWNRIGHT);
                    } else animationBalls[index].setDirection(DOWNLEFT);
                    continue;
                }
            }
        }
        if(!brickEnabled) {
            for(int index = brick[i].animBallIndex; index < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; index++) {
                if(animationBalls[index].getDirection() == STOP) continue;
                for(int j = 0; j < cBricks::brickCount; j++) {
                    if(brick[j].getEnabled() == false) continue;
                    if(CheckCollisionCircleRec(animationBalls[index].getV(), animationBalls[index].getSize(), brick[j].getDimensionsRec())) {
                        if(brick[j].getType() == 5) brick[j].setEnabled(0);
                        else
                            brick[j].callOnCollision();
                        for(int k = 0; k < MAX_POWERUPS; k++) {
                            if(!powerup[k].getEnabled()) {
#ifdef _DEBUG
                                printf("Brick Coord: %f %f\n", brick[j].getX(), brick[j].getY());
#endif // _DEBUG
                                powerup[k].spawnPowerup(&brick[j]);
                                break;
                            }
                        }
                        animationBalls[index].setDirection(STOP);
                        break;
                    }
                }
            }
        }
    }
    //

    for(int i = 0; i < cAnimBall::ballCount; i++) {
        animationBalls[i].Move();
    }

    //Powerups
    for(int i = 0; i < MAX_POWERUPS; i++) {
        if(!powerup[i].getEnabled()) continue;
        if(CheckCollisionRecs(paddle->getDimensionsRec(), powerup[i].getDimensionsRec())) {
            powerup[i].setEnabled(0);
            powerup[i].triggerEffect();
            continue;
        }
        if(powerup[i].getPosition().y > GetScreenHeight() - 10) {
            powerup[i].setEnabled(0);
            continue;
        }
        powerup[i].setPosition(powerup[i].getPosition().x, powerup[i].getPosition().y + GetRandomValue(300, 400) * GetFrameTime());
    }
    //Check powerup status and apply
    if(activePowerups & (1 << 1)) { //+1 Life
        if(lives < 3) lives++;
        activePowerups &= ~(1 << 1);
    }
    if(activePowerups & (1 << 2)) { //Explode
        for(int i = 0; i < cBricks::brickCount; i++) {
            if(brick[i].getType() == 4) {
                brick[i].callOnCollision();
            }
        }
        activePowerups = activePowerups & ~(1 << 2);
    }
    if(activePowerups & (1 << 3)) { //Magnet
        if(ball[0].getDirection() != STOP) {
            if(CheckCollisionCircleRec({ball[0].getX(), ball[0].getY()}, ball[0].getSize(), paddle[0].getDimensionsRec())) {
                ball[0].setY(ball[0].getY() - ball[0].getSize() - 1);
                ball[0].setDirection(STOP);
            }
        }else{
            ball[0].setX(ball[0].getX() + magnetPowerupDiff);
        }
    }
    /*if(activePowerups & (1 << 4)){  //Death

    }
    if(activePowerups & (1 << 5)){  //ShrinkBall

    }
    if(activePowerups & (1 << 6)){  //FastBall

    }
    if(activePowerups & (1 << 7)){  //SuperShrinkPaddle

    }
    if(activePowerups & (1 << 8)){  //FallingBricks

    }
    if(activePowerups & (1 << 9)){ //ExpandPaddle

    }
    if(activePowerups & (1 << 10)){ //ShrinkPaddle

    }
    if(activePowerups & (1 << 11)){ //SplitBall

    }*/

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

void gameReset() {
    lives = 3;
    timer = 0;
    activePowerups = 0;
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
    for(int i = 0; i < 6; i++) {
        powerup[i].setEnabled(0);
    }
    SetRandomSeed(time(NULL));
    LoadLevel();
}

void reset() {
    if(lives < 1) {
        gameReset();
    } else {
        for(int i = 0; i < cBricks::brickCount; i++) {
            if(!brick[i].getEnabled() && brick[i].getType() == 4)
                for(int j = brick[i].animBallIndex; j < brick[i].animBallIndex + STANDARD_ANIM_BALL_COUNT; j++) {
                    animationBalls[j].setDirection(STOP);
                }
        }
        paddle->Reset();
        ball->Reset();
        for(int i = 0; i < 6; i++) {
            powerup[i].setEnabled(0);
        }
        activePowerups = 0;
        lives--;
    }
}

void checkWin() {
    int brickAmount = 0;
    for(int i = 0; i <= cBricks::brickCount; i++) {
        if(brick[i].getType() != 5 || (activePowerups & (1 << 0))) {
            brickAmount = brickAmount + brick[i].getEnabled();
        }
    }
    if(brickAmount == 0) {
        levelCounter++;
        gameReset();
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
