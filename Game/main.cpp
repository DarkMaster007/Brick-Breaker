#include <iostream>
#include <fstream>
#include "include/raylib.h"
#include <string.h>
#include <cstring>
using std::fstream;
using std::endl;
using std::string;
using std::cout;
using std::ios;

fstream loadLevelFile;
fstream game_settings;
#define MAX_BRICKS 300
enum eDir
{ STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};

class cBall
{
private:
    int x, y;
    int originalX, originalY;
    int current_size, originalSize;
    eDir direction;
public:
    cBall(int posX, int posY, int ball_size)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
        originalSize = ball_size;
        current_size = ball_size;
    }
    void Reset()
    {
        x = originalX;
        y = originalY;
        direction = STOP;
        current_size = originalSize;
    }
    void changeDirection(eDir d)
    {
        direction = d;
    }
    void randomDirection()
    {
        direction = (eDir)((rand() % 4) + 1);
    }
    inline int getX()
    {
        return x;
    }
    inline int getY()
    {
        return y;
    }
    inline eDir getDirection()
    {
        return direction;
    }
    inline void changeSize(int ball_size)
    {
        current_size = ball_size;
    }
    inline int getSize()
    {
        return current_size;
    }
    void Move(int movement)
    {
        switch (direction) {
        case STOP:
            break;
        case UPLEFT:
            x-=movement;
            y-=movement;
            break;
        case DOWNLEFT:
            x-=movement;
            y+=movement;
            break;
        case UPRIGHT:
            x+=movement;
            y-=movement;
            break;
        case DOWNRIGHT:
            x+=movement;
            y+=movement;
            break;
        default:
            direction = STOP;
            break;
        }
    }
};

class cPaddle
{
private:
    int x, y;
    int originalX, originalY;
    int paddle_width, paddle_height;
public:
    cPaddle(int posX, int posY)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        paddle_width = 100;
        paddle_height = 10;
    }
    inline int getX()
    {
        return x;
    }
    inline int getY()
    {
        return y;
    }
    void Reset()
    {
        x = originalX;
        y = originalY;
    }
    inline void moveLeft(int pixels)
    {
        x -= pixels;
    }
    inline void moveRight(int pixels)
    {
        x += pixels;
    }
    inline Vector2 getPaddleSize(){
        return {paddle_width, paddle_height};
    }
};

typedef struct Bricks
{
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //0 - Normal, 1 - 1HP, 2 - 2HP, 3 - Gold(Unbreakable), 4 - Explosive
    int enabled;
} Bricks;

class cGameManager
{
private:
    int width,height;               // Screen width and height
    bool fullscreen;                // whether it's full-screen or not
    bool quit;                      // whether the game should quit or not

    Bricks *brick;
    Image imgBrick;
    Texture2D texBrick;
    cBall *ball;                    // ball object
    cPaddle *paddle;                // paddle object
    Rectangle borderLeft;           // left border rectangle, for collision and drawing
    Rectangle borderRight;          // right border rectangle, for collision and drawing
    Rectangle borderTop;            // top border rectangle, for collision and drawing
    Rectangle borderBottom;         // bottom border rectangle, for collision and drawing

    int brickCount;
    int movement_speed_paddle_base; // base speed for paddle
    int movement_speed_ball_base;   // base speed for ball
    int ball_size;                  // ball size
    bool auto_move;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    int exceptions;                 // how many times did the collision NOT work as the code says.
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool collision;                 // whether collision is on or not
    int counter;
    bool pause;

public:
    cGameManager()
    {
        //Open settings.txt file
        game_settings.open("..\\config\\settings.txt", ios::in);

        //Initialize all vars with their proper values
        brickCount = 0;
        quit = 0;
        movement_speed_paddle_base = 20;
        movement_speed_ball_base = 3;
        ball_size = 10;
        auto_move = 0;
        win = 0;
        pause = 0;

        //Load the settings file
        game_settings>>width>>height;
        game_settings>>fullscreen;
        cout<<"Width: "<<width<<endl<<"Height: "<<height;

        InitWindow(width,height,"Editor");
        SetTargetFPS(400);
        ShowCursor();
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, ball_size);
        paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35);
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks)); // MemAlloc() is equivalent to malloc();

        imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
        ImageResize(&imgBrick, 50, 35);
        texBrick = LoadTextureFromImage(imgBrick);

        //Rectangles
        borderLeft = {0, 0, 10, GetScreenHeight()};
        borderRight = {GetScreenWidth() - 10, 0, GetScreenWidth(), GetScreenHeight()};
        borderTop = {0, 0, GetScreenWidth(), 10};
        borderBottom = {0, GetScreenHeight() - 10, GetScreenWidth(), 10};

    }

    void loadLevel()
    {
        loadLevelFile.open("..\\config\\level.txt", ios::in);
        int i = 0;
        while(!loadLevelFile.eof()) {
            loadLevelFile>>brick[i].position.x>>brick[i].position.y>>brick[i].brickWidth>>brick[i].brickHeight>>brick[i].type;
            brick[i].enabled = 1;
            i++;
            brickCount++;
        }
        loadLevelFile.close();
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        //Loop to draw bricks
        Color bColor;
        for(int i = 0; i <= brickCount; i++) {
            if(brick[i].enabled) {
                if(brick[i].type == 0) {
                    bColor = SKYBLUE;
                }
                if(brick[i].type == 1) {
                    bColor = BLUE;
                }
                if(brick[i].type == 2) {
                    bColor = GRAY;
                }
                if(brick[i].type == 3) {
                    bColor = GOLD;
                }
                if(brick[i].type == 4) {
                    bColor = ORANGE;
                }
                //DrawRectangle(brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight, bColor);
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
                DrawLine(brick[i].position.x, brick[i].position.y - 50,brick[i].position.x, brick[i].position.y + 50,RED);
                DrawLine(brick[i].position.x - 50, brick[i].position.y,brick[i].position.x + 50, brick[i].position.y,RED);

            }
        }
        //

        DrawRectangle(0,0,100,100,BLACK);

        // Draw border
        DrawRectangleRec(borderLeft,BROWN);
        DrawRectangleRec(borderRight,BROWN);
        DrawRectangleRec(borderTop,BROWN);
        DrawRectangleRec(borderBottom,BROWN);

        // Paddle draw
        DrawRectangle(paddle->getX(),paddle->getY(),paddle->getPaddleSize().x,paddle->getPaddleSize().y,WHITE);

        //Ball draw
        int ballX = ball->getX();
        int ballY = ball->getY();
        int ballRad = 10;
        DrawCircle(ballX,ballY,ballRad,WHITE);
        EndDrawing();
    }

    void Input()
    {
        float frametime = GetFrameTime() * 100;
        int movement_speed_paddle = movement_speed_paddle_base * frametime;
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5) {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        //Mouse movement
        if(!auto_move) {
            if(paddle->getX() - movement_speed_paddle - 50 > 10) {
                if(GetMouseDelta().x < 0) {
                    paddle->moveLeft(movement_speed_paddle);
                }
                if(IsKeyDown('A')) {
                    paddle->moveLeft(movement_speed_paddle);
                }
            }
            if(paddle->getX() + movement_speed_paddle + 50 < GetScreenWidth() - 10) {
                if(GetMouseDelta().x > 0) {
                    paddle->moveRight(movement_speed_paddle);
                }
                if(IsKeyDown('D')) {
                    paddle->moveRight(movement_speed_paddle);
                }
            }
        }
        SetMousePosition(GetScreenWidth() / 2,GetMousePosition().y);
        //

        if(IsKeyPressed('W')) {         // Enable auto-moving
            auto_move = !auto_move;
        }

        if(IsKeyPressed('R')) {         // Reset the level
            reset();
        }

        if(IsKeyPressed('P')){
            pause = !pause;
        }

        if(ball->getDirection() == STOP) {                      // Click to start the ball movement thing
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ball->changeDirection(UPRIGHT);
            }
        }
    }

    void Logic()
    {
        Vector2 ball_collision = {ball->getX(), ball->getY()};
        float frametime = GetFrameTime() * 100;

        int movement_speed_paddle = movement_speed_paddle_base * frametime;
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5) {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        if(auto_move) {
            if(paddle->getX() - movement_speed_paddle > 10) {
                if(ball->getX() < paddle->getX() + 25) {
                    paddle->moveLeft(movement_speed_paddle);
                }
            }
            if(paddle->getX() + movement_speed_paddle + paddle->getPaddleSize().x < GetScreenWidth() - 10) {
                if(ball->getX() > paddle->getX() + 25) {
                    paddle->moveRight(movement_speed_paddle);
                }
            }
        }


        int movement_speed_ball = (movement_speed_ball_base + rand() % 6) * frametime;
        if(movement_speed_ball == 0) {
            movement_speed_ball = 1;
        }
        if(movement_speed_ball > movement_speed_ball_base * 2.5) {
            movement_speed_ball = movement_speed_ball_base * 2.5;
        }
        if(!pause){
        ball->Move(movement_speed_ball);}

        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), borderLeft)) {         //left wall
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(DOWNRIGHT);
            }
        }

        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderRight)) {         //right wall
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT) {
                ball->changeDirection(DOWNLEFT);
            }
        }

        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderTop)) {           //top wall
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(DOWNRIGHT);
            }
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(DOWNLEFT);
            }
        }

        //Paddle Collision
        ball_collision = {ball->getX(), ball->getY()};
        Rectangle paddle_collision = {paddle->getX(), paddle->getY(),paddle->getPaddleSize().x, paddle->getPaddleSize().y};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision)) {
            if(ball->getX() < paddle->getX() + paddle->getPaddleSize().x / 2){
                if(ball->getDirection() == DOWNLEFT){
                    ball->changeDirection(UPLEFT);
                }
                else if(ball->getDirection() == DOWNRIGHT){
                    ball->changeDirection(UPLEFT);
                }
            }
            else {
                if(ball->getDirection() == DOWNRIGHT){
                    ball->changeDirection(UPRIGHT);
                }
                else if(ball->getDirection() == DOWNLEFT){
                    ball->changeDirection(UPRIGHT);
                }
            }
        }

        if(collision == 0) {
            timer(6);
        }
        if(counter == 0) {
            collision = 1;
        }
        ball_collision = {ball->getX(), ball->getY()};
        Rectangle brick_collision;
        for(int i = 0; i <= brickCount; i++){
        brick_collision = {brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight};
        if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && brick[i].enabled){
                if(ball->getX() <= brick[i].position.x){
                    if(ball->getDirection() == UPRIGHT){
                        ball->changeDirection(UPLEFT);
                    }
                    else {ball->changeDirection(DOWNLEFT);}
                }
                else if(ball->getX() >= brick[i].position.x + brick[i].brickWidth){
                    if(ball->getDirection() == UPLEFT){
                        ball->changeDirection(UPRIGHT);
                    }
                    else {ball->changeDirection(DOWNRIGHT);}
                }
                else if(ball->getY() <= brick[i].position.y){
                    if(ball->getDirection() == DOWNLEFT){
                        ball->changeDirection(UPLEFT);
                    }
                    else {ball->changeDirection(UPRIGHT);}
                }
                else if(ball->getY() >= brick[i].position.y + brick[i].brickHeight){
                    if(ball->getDirection() == UPRIGHT){
                        ball->changeDirection(DOWNRIGHT);
                    }
                    else {ball->changeDirection(DOWNLEFT);}
                }
                brick[i].enabled = 0;
           }
        }
    }

    void reset()
    {
        paddle->Reset();
        ball->Reset();
        loadLevel();
    }

    void checkWin()
    {
        if(brickCount == 0){
            win = 1;
        }
    }

    void timer(int delayframes)
    {
        if(counter<delayframes) {
            counter++;
        } else {
            counter = 0;
        }
    }

    int Run()
    {
        loadLevel();
        while(!WindowShouldClose() & !win) {
                Input();
                Logic();
                checkWin();
            Draw();
        }
        cout<<"Exception rate: "<<(float)exceptions/brickCount*100<<" %."<<endl;
        MemFree(brick);
        CloseWindow();
        return 0;
    }

};

int main()
{
    cGameManager cGame;
    int exit = cGame.Run();
    return exit;
}
