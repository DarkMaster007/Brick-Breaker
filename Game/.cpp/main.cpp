#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include <raygui.h>
#include <stdio.h>
#include "soundDeath.h"
#include "soundSelect.h"
#include "soundBounceGeneral.h"
#include "soundBouncePaddle.h"
#include <math.h>
#include <time.h>
#include <random>
#include <chrono>

#define MAX_BRICKS 300
#define MAX_POWERUPS 6

enum eDir{ STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};
enum eActivePowerups{ LASER = 0, CROSSHAIR = 1, EXTEND = 2, PIERCE = 3, FIREBALL = 4, NEXTLEVEL = 5 };

class cBall {
private:
    float x, y;
    float originalX, originalY;
    int current_size, originalSize;
    float speed{400};
    eDir direction;
    float randomMovementOffset[2]{};
    std::mt19937 marsenneTwister{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> randomNrDistribution{1, 4};
    std::uniform_int_distribution<> randomNrDirection{1, 4};
public:
    cBall(int posX, int posY, int ball_size) {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
        originalSize = ball_size;
        current_size = ball_size;
        randomizeMovement();
    }
    void Reset() {
        x = originalX;
        y = originalY;
        direction = STOP;
        current_size = originalSize;
    }
    void changeDirection(eDir d) {
        direction = d;
    }
    void randomDirection() {
        direction = (eDir)(randomNrDirection(marsenneTwister));
    }
    inline int getX() {
        return x;
    }
    inline int getY() {
        return y;
    }
    inline eDir getDirection() {
        return direction;
    }
    inline void changeSize(int ball_size) {
        current_size = ball_size;
    }
    inline int getSize() {
        return current_size;
    }
    inline void randomizeMovement(){
        randomMovementOffset[0] = randomNrDistribution(marsenneTwister) / 10.0f;
        randomMovementOffset[1] = randomNrDistribution(marsenneTwister) / 10.0f;
    }
    void Move() {
        switch (direction) {
        case STOP:
            break;
        case UPLEFT:
            x-=(speed + randomMovementOffset[0]) * GetFrameTime();
            y-=(speed + randomMovementOffset[1]) * GetFrameTime();
            break;
        case DOWNLEFT:
            x-=(speed + randomMovementOffset[0]) * GetFrameTime();
            y+=(speed + randomMovementOffset[1]) * GetFrameTime();
            break;
        case UPRIGHT:
            x+=(speed + randomMovementOffset[0]) * GetFrameTime();
            y-=(speed + randomMovementOffset[1]) * GetFrameTime();
            break;
        case DOWNRIGHT:
            x+=(speed + randomMovementOffset[0]) * GetFrameTime();
            y+=(speed + randomMovementOffset[1]) * GetFrameTime();
            break;
        default:
            direction = STOP;
            break;
        }
    }
};

typedef struct powerup {
    eActivePowerups type;
    Vector2 position;
    bool enabled;
} powerup;

class cPaddle {
private:
    int x, y;
    int originalX, originalY;
    int paddle_width, paddle_height;
    int bounceReverseArea;           // How big the area is where the ball goes back instead of forward
public:
    cPaddle(int posX, int posY, int width, int height) {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        paddle_width = width;
        paddle_height = height;
        bounceReverseArea = 13;
    }
    inline int getX() {
        return x;
    }
    inline int getY() {
        return y;
    }
    void Reset() {
        x = originalX;
        y = originalY;
    }
    inline void moveLeft(int pixels) {
        x -= pixels;
    }
    inline void moveRight(int pixels) {
        x += pixels;
    }
    inline Vector2 getSize() {
        return {(float)paddle_width, (float)paddle_height};
    }
    inline int getBounceReverseArea() {
        return bounceReverseArea;
    }
};

typedef struct Bricks {
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    bool enabled;
} Bricks;

class cGameManager {
private:
    FILE *fp;
    Bricks *brick;
    Image imgBrick;
    Texture2D texBrick;
    Texture2D texBall;
    Texture2D texPaddleEdge_L;
    Texture2D texPaddleEdge_R;
    Texture2D texPaddleBody;
    Texture2D texPowerup;
    cBall *ball;                    // ball object
    cPaddle *paddle;                // paddle object
    Rectangle borderLeft;           // left border rectangle, for collision and drawing
    Rectangle borderRight;          // right border rectangle, for collision and drawing
    Rectangle borderTop;            // top border rectangle, for collision and drawing
    Rectangle borderBottom;         // bottom border rectangle, for collision and drawing
    powerup *c_powerup;              // Handles powerups

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

    int width,height;               // Screen width and height
    bool fullscreen;                // whether it's full-screen or not
    bool quit;                      // whether the game should quit or not
    int brickCount;
    float movement_speed_paddle_base; // base speed for paddle
    float movement_speed_ball_base;   // base speed for ball
    bool auto_move;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool pause;

public:
    cGameManager() {
        SetRandomSeed(time(NULL));
        fp = NULL;
        //Open settings.txt file
        fp = fopen("..\\config\\settings.txt", "r");
        //Initialize all vars with their proper values
        brickCount = 0;
        quit = 0;
        movement_speed_paddle_base = 8;
        movement_speed_ball_base = 1.6;
        auto_move = 0;
        win = 0;
        pause = 0;

        //Load the settings file
        fscanf(fp, "%d %d %d", &width, &height, (int*)&fullscreen);
        //printf("Width: %d\nHeight: %d\nFullscreen: %d\n", width, height, fullscreen);

        fclose(fp);

        InitWindow(width,height,"Editor");
        InitAudioDevice();
        SetTargetFPS(120);
        HideCursor();
        if(fullscreen == 1) {
            ToggleFullscreen();
        }
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, 10);
        paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks));  //MemAlloc() is equivalent to malloc();
        c_powerup = (powerup *)MemAlloc(6*sizeof(powerup));

        //Load and resize image and then convert it to texture
        imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
        Image imgBall = LoadImage("..//resources//Breakout-Ball.gif");
        ImageResize(&imgBall, ball->getSize() * 2, ball->getSize() * 2);
        texBall = LoadTextureFromImage(imgBall);
        texPaddleEdge_L = LoadTexture("..//resources//Breakout-Paddle_L.png");
        texPaddleEdge_R = LoadTexture("..//resources//Breakout-Paddle_R.png");
        texPaddleBody = LoadTexture("..//resources//Breakout-Paddle_middle.png");
        texPowerup = LoadTexture("..//resources//powerup_placeholder.png");
        Image imgPowerup = LoadImage("..//resources//powerup_placeholder.png");
        ImageResize(&imgPowerup, GetScreenWidth() * 0.04, GetScreenHeight() * 0.04);
        texPowerup = LoadTextureFromImage(imgPowerup);

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

    void loadLevel() {
        fp = fopen("..\\config\\level.txt", "r");
        int i = 0;
        int temp, temp2;
        int level_width, level_height;
        brickCount = 0;
        while(!feof(fp)) {
            fscanf(fp, "%d %d %f %f %d %d %d", &level_width, &level_height, &brick[i].position.x, &brick[i].position.y, &brick[i].brickWidth, &brick[i].brickHeight, &brick[i].type);
            if(width != level_width || height != level_height) {
                temp = width / level_width;
                temp2 = height / level_height;
                brick[i].position.x *= temp;
                brick[i].position.y *= temp2;
                brick[i].brickWidth *= (temp*1.5);
                brick[i].brickHeight *= (temp2*1.2);
                //cout<<"X mod: "<<temp<<endl<<"Y mod: "<<temp2<<endl;
                //cout<<"Level res to game res ratio is: "<<temp<<" to "<<temp2<<endl;
            }
            //X clamping
            if(brick[i].position.x < 10) {
                brick[i].position.x += 50;
                if(brick[i].position.x < 10) {
                    brick[i].position.x = 11;
                }
            }
            if(brick[i].position.x + brick[i].brickWidth > width - 10) {
                brick[i].position.x -= 50;
                if(brick[i].position.x < 10) {
                    brick[i].position.x = width - 11 - brick[i].brickWidth;
                }
            }
            //
            //Y clamping
            if(brick[i].position.y + brick[i].brickHeight > GetScreenHeight() - 350) {
                brick[i].position.y -= 25;
                if(brick[i].position.y > GetScreenHeight() - 350) {
                    brick[i].position.y = GetScreenHeight() - 351 - brick[i].brickHeight;
                }
            }
            if(brick[i].position.y < 0) {
                brick[i].position.y += 50;
                if(brick[i].position.y < 0) {
                    brick[i].position.y = 11;
                }
            }
            //
            brick[i].type++;
            brick[i].enabled = true;
            i++;
            brickCount++;
        }
        ImageResize(&imgBrick, brick[0].brickWidth, brick[0].brickHeight);
        texBrick = LoadTextureFromImage(imgBrick);
        fclose(fp);
    }

    void SpawnPowerup(int brickIndex) {
        int change_percent = 35;        // change this to change chance; I think that's how this works right?
        int generated_num = GetRandomValue(0, 1000);
        if(generated_num <= change_percent * 10) {
            if(!brick[brickIndex].enabled) {
                int i = 0;
                bool changed = 0;
                do {
                    if(c_powerup[i].enabled == 0) {
                        printf("Powerup enabled!");
                        c_powerup[i].enabled = 1;
                        c_powerup[i].type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
                        printf(" Type: %i\n", c_powerup[i].type);
                        changed = 1;                    // TODO (DarkMaster#4#10/12/22): To change, I don't think "changed" does anything
                        c_powerup[i].position.x = brick[brickIndex].position.x;
                        c_powerup[i].position.y = brick[brickIndex].position.y;
                    }
                    i++;
                } while(changed == 0 && i < 6);
            }
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);

        //Loop to draw bricks
        Color bColor;
        for(int i = 0; i < brickCount - 1; i++) {
            if(brick[i].enabled) {
                if(brick[i].type == 1) {
                    bColor = SKYBLUE;
                }
                if(brick[i].type == 2) {
                    bColor = BLUE;
                }
                if(brick[i].type == 3) {
                    bColor = GRAY;
                }
                if(brick[i].type == 5) {
                    bColor = GOLD;
                }
                if(brick[i].type == 4) {
                    bColor = ORANGE;
                }
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
            }
        }
        //

        // Draw border
        DrawRectangleRec(borderTop,BROWN);
        DrawRectangleRec(borderBottom,RED);
        DrawRectangleRec(borderLeft,BROWN);
        DrawRectangleRec(borderRight,BROWN);

        // Paddle draw
        DrawRectangle(paddle->getX(),paddle->getY(),paddle->getSize().x,paddle->getSize().y,WHITE);

        DrawRectangle(paddle->getX(), paddle->getY(), paddle->getBounceReverseArea(), paddle->getSize().y, YELLOW);
        DrawRectangle(paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea(), paddle->getY(), paddle->getBounceReverseArea(), paddle->getSize().y, YELLOW);

        Rectangle Source = {0, 0, (float)texPaddleEdge_L.width, (float)texPaddleEdge_L.height};
        Rectangle Destination = {(float)paddle->getX(), (float)paddle->getY(), (float)paddle->getBounceReverseArea(), paddle->getSize().y};
        DrawTexturePro(texPaddleEdge_L, Source, Destination, {0,0}, 0, WHITE);

        Source = {0, 0, (float)texPaddleEdge_R.width, (float)texPaddleEdge_R.height};
        Destination = {(float)(paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea()), (float)paddle->getY(), (float)paddle->getBounceReverseArea(), paddle->getSize().y};
        DrawTexturePro(texPaddleEdge_R, Source, Destination, {0,0}, 0, WHITE);

        Source = {0, 0, (float)texPaddleBody.width, (float)texPaddleBody.height};
        Destination = {(float)(paddle->getX() + paddle->getBounceReverseArea()), (float)paddle->getY(), paddle->getSize().x -  2 * paddle->getBounceReverseArea(), paddle->getSize().y};
        DrawTexturePro(texPaddleBody, Source, Destination, {0,0}, 0, WHITE);

        //Ball draw
        DrawTexture(texBall, ball->getX() - ball->getSize(), ball->getY() - ball->getSize(), WHITE);

        //Powerup / powerdown draw
        for(int i = 0; i < 6; i++) {
            if(c_powerup[i].enabled) {
                int movement_speed_powerup = movement_speed_ball_base + GetRandomValue(0, 5);
                movement_speed_powerup = movement_speed_powerup + movement_speed_powerup * GetFrameTime();
                //printf("Powerup drawn!\n");
                DrawTexture(texPowerup, c_powerup[i].position.x - 50, c_powerup[i].position.y - 25, WHITE);
                c_powerup[i].position.y += movement_speed_powerup;
            }
        }

        //Debug stuff here:
        DrawFPS(10,10);
        float temp = GetFrameTime();
        GuiStatusBar({0, GetScreenHeight() - 30, 100, 30}, TextFormat("%f", std::abs(temp)));

        EndDrawing();
    }

    void Input() {
        int movement_speed_paddle = movement_speed_paddle_base;
        movement_speed_paddle = movement_speed_paddle + movement_speed_paddle * GetFrameTime();
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5) {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        //Mouse movement
        if(!auto_move && !pause) {
            if(paddle->getX() - movement_speed_paddle >= 10) {
                if(GetMouseDelta().x < 0 || IsKeyDown('A')) {
                    paddle->moveLeft(movement_speed_paddle + abs(GetMouseDelta().x));
                }
            }
            if(paddle->getX() + movement_speed_paddle + paddle->getSize().x <= GetScreenWidth() - 10) {
                if(GetMouseDelta().x > 0 || IsKeyDown('D')) {
                    paddle->moveRight(movement_speed_paddle + abs(GetMouseDelta().x));
                }
            }
        }
        //

        //Lock mouse position so it doesn't go outside of screen
        if(!pause) {
            SetMousePosition(GetScreenWidth() / 2,GetScreenHeight() / 2);
        }


        // Enable auto-moving
        if(IsKeyPressed('W')) {
            auto_move = !auto_move;
        }

        // Reset the level
        if(IsKeyPressed('R')) {
            reset();
        }

        //Pause the game
        if(IsKeyPressed('P')) {
            pause = !pause;
        }

        if(IsKeyPressed('F')){
            ToggleFullscreen();
        }

        if(IsKeyPressed('M')){
            soundMute = !soundMute;
            SetMasterVolume(soundVolume * !soundMute);
        }

        // Click to start the ball movement thing
        if(ball->getDirection() == STOP) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ball->changeDirection(UPRIGHT);
            }
        }
    }

    Vector2 BrickCollision() {
        //Brick Collision
        Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};
        Rectangle brick_collision;
        for(int i = 0; i < brickCount - 1; i++) {
            brick_collision = {brick[i].position.x, brick[i].position.y, (float)brick[i].brickWidth, (float)brick[i].brickHeight};
            if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && brick[i].enabled) {
                if(ball->getX() <= brick[i].position.x) {
                    if(ball->getDirection() == UPRIGHT) {
                        ball->changeDirection(UPLEFT);
                    } else {
                        ball->changeDirection(DOWNLEFT);
                    }
                } else if(ball->getX() >= brick[i].position.x + brick[i].brickWidth) {
                    if(ball->getDirection() == UPLEFT) {
                        ball->changeDirection(UPRIGHT);
                    } else {
                        ball->changeDirection(DOWNRIGHT);
                    }
                } else if(ball->getY() <= brick[i].position.y) {
                    if(ball->getDirection() == DOWNLEFT) {
                        ball->changeDirection(UPLEFT);
                    } else {
                        ball->changeDirection(UPRIGHT);
                    }
                } else if(ball->getY() >= brick[i].position.y + brick[i].brickHeight) {
                    if(ball->getDirection() == UPRIGHT) {
                        ball->changeDirection(DOWNRIGHT);
                    } else {
                        ball->changeDirection(DOWNLEFT);
                    }
                }
                if(brick[i].type == 4) {
                    brick[i].type = 0;
                } else if(brick[i].type != 5) {   //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
                    brick[i].type -= 1;
                }
                if(brick[i].type == 0) {
                    brick[i].enabled = 0;
                }
                SpawnPowerup(i);
                PlaySound(soundBounceGeneral);
                ball->randomizeMovement();
            }
        }
        return ball_collision;
    }

    void Logic() {
        Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};

        int movement_speed_paddle = movement_speed_paddle_base;
        movement_speed_paddle = movement_speed_paddle + movement_speed_paddle * GetFrameTime();
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5) {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        //Auto-movement for the paddle (doesn't work well anymore since I added the ability to send the ball back in same direction)
        if(auto_move) {
            if(paddle->getX() - movement_speed_paddle > 10) {
                if(ball->getX() < paddle->getX() + 25) {
                    paddle->moveLeft(movement_speed_paddle);
                }
            }
            if(paddle->getX() + movement_speed_paddle + paddle->getSize().x < GetScreenWidth() - 10) {
                if(ball->getX() > paddle->getX() + 25) {
                    paddle->moveRight(movement_speed_paddle);
                }
            }
        }

        //Move only if game is NOT paused
        if(!pause) {
            ball->Move();
        }

        //Left wall collision
        ball_collision = {(float)ball->getX(), (float)ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), borderLeft)) {
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(DOWNRIGHT);
            }
            PlaySound(soundBounceGeneral);
        }

        //Right wall collision
        ball_collision = {(float)ball->getX(), (float)ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderRight)) {
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT) {
                ball->changeDirection(DOWNLEFT);
            }
            PlaySound(soundBounceGeneral);
        }

        //Top wall collision
        ball_collision = {(float)ball->getX(), (float)ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderTop)) {
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(DOWNRIGHT);
            }
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(DOWNLEFT);
            }
            PlaySound(soundBounceGeneral);
        }

        //Bottom wall collision
        ball_collision = {(float)ball->getX(), (float)ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom)) {
            //cout<<"You Lose!"<<endl;
            PlaySound(soundDeath);
            reset();
        }


        //Paddle Collision
        ball_collision = {(float)ball->getX(), (float)ball->getY()};
        Rectangle paddle_collision = {(float)paddle->getX(), (float)paddle->getY(), paddle->getSize().x, paddle->getSize().y};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision)) {
            if(paddle->getSize().x > 30) {
                if(ball->getX() < paddle->getX() + paddle->getBounceReverseArea()) {
                    ball->changeDirection(UPLEFT);
                    //cout<<"LEFT"<<endl;
                }
                if(ball->getX() > paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea()) {
                    ball->changeDirection(UPRIGHT);
                    //cout<<"RIGHT"<<endl;
                }
                if(ball->getX() >= paddle->getX() + paddle->getBounceReverseArea() && ball->getX() <= paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea()) {
                    if(ball->getDirection() == DOWNLEFT) {
                        ball->changeDirection(UPLEFT);
                    } else {
                        ball->changeDirection(UPRIGHT);
                    }
                    //cout<<"MIDDLE"<<endl;
                }
            } else {
                if(ball->getDirection() == DOWNLEFT) {
                    ball->changeDirection(UPLEFT);
                } else ball->changeDirection(UPRIGHT);
            }
            while(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision)) {
                if(!pause) {
                    Draw();
                    ball->Move();
                    ball_collision = BrickCollision();
                }
            }
            PlaySound(soundBouncePaddle);
        }

        //Brick Collision
        ball_collision = BrickCollision();

        for(int i = 0; i < 6; i++) {
            if(c_powerup[i].position.y > GetScreenHeight() - 10) {
                c_powerup[i].enabled = 0;
            }
        }
    }

    void reset() {
        paddle->Reset();
        ball->Reset();
        loadLevel();
        for(int i = 0; i < 6; i++) {
            c_powerup[i].enabled = 0;
        }
        SetRandomSeed(time(NULL));
    }

    void checkWin() {
        int a = 0;
        for(int i = 0; i <= brickCount; i++) {
            if(brick[i].type != 5) {
                a = a + brick[i].enabled;
            }
        }
        a -= 1;
        if(a == 0) {
            //win = 1;
            //cout<<"WIN!!!!"<<endl;
            reset();
        }
    }

    void cleanup() {
        MemFree(brick);
        MemFree(c_powerup);
        delete[] ball;
        delete[] paddle;
        _fcloseall();
        CloseWindow();
    }

    int Run() {
        loadLevel();
        while(!WindowShouldClose() & !win) {
            Input();
            Logic();
            checkWin();
            Draw();
        }
        cleanup();
        return 0;
    }

};

int main() {
    cGameManager cGame;
    int exit = cGame.Run();
    return exit;
}
