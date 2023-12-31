#define RAYGUI_IMPLEMENTATION
#include "defines.h"
#include "DrawFunctions.h"
#include "cBricks.h"
#include "cBall.h"
#include "cPaddle.h"
#include "cPowerup.h"

FILE *fp;

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
    Init();
    if(LoadLevel() != 0) {
        exitCode = -1;
    };
    while(!WindowShouldClose() & !win) {
        Input();
        Logic();
        checkWin();
        Draw();
    }
    Unload();
    return exitCode;
}

void Init() {
    int width, height;

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

    fp = fopen("..\\config\\settings.txt", "r"); //Open settings.txt file
    fscanf(fp, "%d %d %d", &width, &height, (int*)&fullscreen); //Load the settings file
    fclose(fp);

#ifdef _DEBUG
    printf("Width: %d\nHeight: %d\nFullscreen: %d\n", width, height, fullscreen);
#endif // _DEBUG

    SetRandomSeed(time(NULL)); //Set seed for random values with current time
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(width,height,"Editor");
    InitAudioDevice();
    SetTargetFPS(120); // TODO (Administrator#7#): Make everything use deltaTime so you can remove the FPS limit
    HideCursor();
    if(fullscreen == 1) {
        ToggleFullscreen();
    }

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
    borderLeft = {0, 0, 10, (float)GetScreenHeight()};
    borderRight = {(float)(GetScreenWidth() - 10), 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    borderTop = {0, 0, (float)GetScreenWidth(), 10};
    borderBottom = {0, (float)(GetScreenHeight() - 10), (float)GetScreenWidth(), 10};
}

int LoadLevel() {
    int i = 0;
    int tempWidth, tempHeight;
    int levelWidth, levelHeight;
    float brickPosX, brickPosY;
    int brickWidth, brickHeight, brickType;

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
    while(!feof(fp)) {
        fscanf(fp, "%d %d %f %f %d %d %d", &levelWidth, &levelHeight, &brickPosX, &brickPosY, &brickWidth, &brickHeight, &brickType);
        if(GetScreenWidth() != levelWidth || GetScreenHeight() != levelHeight) {
            tempWidth = GetScreenWidth() / levelWidth;
            tempHeight = GetScreenHeight() / levelHeight;
            brickPosX *= tempWidth;
            brickPosY *= tempHeight;
            brickWidth *= (tempWidth*1.5);
            brickHeight *= (tempHeight*1.2);
#ifdef _DEBUG
            printf("X mod: %d/nY mod: %d/n",tempWidth,tempHeight);
            printf("Level res to game res ratio is: %d to %d./n",tempWidth,tempHeight);
#endif // _DEBUG
        }
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
    if(cAnimBall::ballCount % STANDARD_ANIM_BALL_COUNT != 0) throw "Invalid animation ball count";
    fclose(fp);
    return 0;
}

void Draw() {
    BeginDrawing();
    if(ball->getDirection() != STOP && !isPaused) timer += GetFrameTime();
    double currentTime = GetTime();
    if(currentTime - previousTime >= updateInterval) {
        if(animationFrame >= 100) animationFrame = 0;
        if(!isPaused) {
            animationFrame++;
        }
        previousTime = currentTime;
    }
    ClearBackground(BLACK);

    //cBricks::Draw(brick, animationFrame);
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
        }
        else{
            if(brick[i].getType() == 4){
                for(int j = brick[i].animBallIndex; j < brick[i].animBallIndex + 6; j++){
                    animationBalls[j].Move(); // TODO (DarkMaster#9#12/31/23): Move this shit to LOGIC
                    cAnimBall::Draw(&animationBalls[j]);
                }
            }
        }
    }

    cPaddle::Draw(paddle);

    cBall::Draw(ball);

    cPowerup::Draw(powerup);

    // Draw border
    DrawRectangleRec(borderTop,BROWN);
    DrawRectangleRec(borderLeft,BROWN);
    DrawRectangleRec(borderRight,BROWN);
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

    ball->Logic(timer, isPaused);

    paddle->Logic(ball, autoMove, isPaused, soundBouncePaddle);

    for(int i = 0; i < cBricks::brickCount; i++) {
        brick[i].Logic(ball, powerup, soundBounceGeneral);
    }

    for(int i = 0; i < MAX_POWERUPS; i++) {
        powerup[i].Logic();
    }

    //Make bricks fall after 60 seconds
    // TODO (Administrator#8#): Change how timer works so that it stops when pausing
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
    for(int i = 0; i < cBricks::brickCount; i++) {
        brick[i].~cBricks();
    }
    MemFree(brick);
    MemFree(animationBalls);
    delete[] powerup;
    delete ball;
    delete paddle;
    _fcloseall();
    CloseWindow();
    printf("Destructor finished.\n");
}
