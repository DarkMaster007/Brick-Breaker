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
public:
    cPaddle(int posX, int posY)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
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
    int auto_move;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    int exceptions;                 // how many times did the collision NOT work as the code says.
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool collision;                 // whether collision is on or not
    int counter;

public:
    cGameManager()
    {
        //Open settings.txt file
        game_settings.open("..//config//settings.txt", ios::in);

        //Initialize all vars with their proper values
        brickCount = 0;
        quit = 0;
        movement_speed_paddle_base = 20;
        movement_speed_ball_base = 3;
        ball_size = 10;
        auto_move = 0;
        exceptions = 0;
        win = 0;
        collision = 1;
        counter = 0;
        Image imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
        ImageResize(&imgBrick, 50, 35);
        texBrick = LoadTextureFromImage(imgBrick);

        //Load the settings file
        game_settings>>width>>height;
        game_settings>>fullscreen;

        InitWindow(width,height,"Editor");
        SetTargetFPS(400);
        ShowCursor();
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, ball_size);
        paddle = new cPaddle(GetScreenWidth() / 2, GetScreenHeight() - 35);
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks)); // MemAlloc() is equivalent to malloc();

        //Rectangles
        borderLeft = {0, 0, 10, GetScreenHeight()};
        borderRight = {GetScreenWidth() - 10, 0, GetScreenWidth(), GetScreenHeight()};
        borderTop = {0, 0, GetScreenWidth(), 10};
        borderBottom = {0, GetScreenHeight() - 10, GetScreenWidth(), 10};

    }

    void loadLevel()
    {
        loadLevelFile.open("..//config//level.txt", ios::in);
        int i;
            while(loadLevelFile.peek() != EOF){
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
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
            }
        }
        //

        // Draw border
        DrawRectangleRec(borderLeft,BROWN);
        DrawRectangleRec(borderRight,BROWN);
        DrawRectangleRec(borderTop,BROWN);
        DrawRectangleRec(borderBottom,BROWN);

        // Paddle draw
        int paddleX = paddle->getX();
        int paddleY = paddle->getY();
        DrawRectangle(paddleX - 50,paddleY,100,10,WHITE);

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

        if(ball->getDirection() == STOP) {                      // Click to start the ball movement thing
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ball->changeDirection(UPRIGHT);
            }
        }
    }

    void Logic()
    {
        float frametime = GetFrameTime() * 100;
        int movement_speed_paddle = movement_speed_paddle_base * frametime;
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5) {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        if(auto_move) {
            if(paddle->getX() - movement_speed_paddle - 50 > 10) {
                if(ball->getX() < paddle->getX() + 25) {
                    paddle->moveLeft(movement_speed_paddle);
                }
            }
            if(paddle->getX() + movement_speed_paddle + 50 < GetScreenWidth() - 10) {
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
        ball->Move(movement_speed_ball);

        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), borderLeft)) {         //left wall
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(DOWNRIGHT);
            }
        }
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(),borderRight)) {         //right wall
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT) {
                ball->changeDirection(DOWNLEFT);
            }
        }
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(),borderTop)) {           //top wall
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(DOWNRIGHT);
            }
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(DOWNLEFT);
            }
        }

        //Paddle Collision
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), {paddle->getX() - 50, paddle->getY(),100, 10})) {
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT) {
                ball->changeDirection(UPRIGHT);
            }
        }

        if(collision == 0){
            timer(6);
        }
        if(counter == 0){
            collision = 1;
        }
        //Collision for bricks
        if(collision) {
            for(int i = 0; i < brick_columns; i++) {
                for(int j = 0; j < brick_rows; j++) {
                    if(brick[i][j]) {
                        int x = brick_centering + (offset * 2) + i * brick_width + i * offset;
                        int y = offset * 2 + j * brick_height + j * offset;
                        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), {x, y, brick_width, brick_height})) {
                            if(brick[i][j] <= 3){
                            brick[i][j] --;}
                            collision = 0;

                            if(ball->getX() < x + 5 && ball->getY() > y && ball->getY() < (y + brick_height)) { //Left brick side collision
                                if(ball->getDirection() == UPRIGHT) {
                                    ball->changeDirection(UPLEFT);
                                } else {
                                    if(ball->getDirection() == DOWNRIGHT) {
                                        ball->changeDirection(DOWNLEFT);
                                    }
                                }
                            } else if(ball->getX() > (x + brick_width) && ball->getY() > y && ball->getY() < (y + brick_height)) { //Right brick side collision
                                if(ball->getDirection() == UPLEFT) {
                                    ball->changeDirection(UPRIGHT);
                                } else {
                                    if(ball->getDirection() == DOWNLEFT) {
                                        ball->changeDirection(DOWNRIGHT);
                                    }
                                }
                            } else if(ball->getY() < y && ball->getX() > x && ball->getX() < (x + brick_width)) { //Top brick side collision
                                if(ball->getDirection() == DOWNLEFT) {
                                    ball->changeDirection(UPLEFT);
                                }
                                if(ball->getDirection() == DOWNRIGHT) {
                                    ball->changeDirection(UPRIGHT);
                                }
                            } else if(ball->getY() > (y + brick_height) && ball->getX() > x && ball->getX() < (x + brick_width)) { //Bottom brick side collision
                                if(ball->getDirection() == UPLEFT) {
                                    ball->changeDirection(DOWNLEFT);
                                }
                                if(ball->getDirection() == UPRIGHT) {
                                    ball->changeDirection(DOWNRIGHT);
                                }
                            } else {
                                if(ball->getX() < (x + brick_width / 2) && ball->getX() > (x - ball_size - 5)) {
                                    if(ball->getDirection() == DOWNRIGHT) {
                                        ball->changeDirection(DOWNLEFT);
                                    }
                                    if(ball->getDirection() == UPRIGHT) {
                                        ball->changeDirection(UPLEFT);
                                    }
                                } else if(ball->getX() > (x + brick_width / 2) && ball->getX() < (x + brick_width + ball_size + 5)) {
                                    if(ball->getDirection() == UPLEFT) {
                                        ball->changeDirection(UPRIGHT);
                                    }
                                    if(ball->getDirection() == DOWNLEFT) {
                                        ball->changeDirection(DOWNRIGHT);
                                    }
                                } else {
                                    ball->randomDirection();
                                    exceptions++;
                                    cout<<"Total exceptions: "<<exceptions<<endl;
                                    char test[11];
                                    sprintf(test, "Test %d.png", exceptions);
                                    TakeScreenshot(test);
                                }
                            }
                        }
                    }
                }
            }
        }

        if(collision){

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
        int sum = 0;
        for(int i = 0; i < brick_columns; i++) {
                if(brick[i].type != 3){
                sum = sum + brick[i].enabled;}
        }
        if(sum == 0) {
            //win = 1;
            reset();
        }
    }

    void timer(int delayframes){
        if(counter<delayframes){
            counter++;
        }
        else{
            counter = 0;
        }
    }

    int Run()
    {
        loadLevel();
        while(!WindowShouldClose() & !win) {
            if(IsWindowFocused()) {
                Input();
                Logic();
                checkWin();
            }
            Draw();
        }
        cout<<"Exception rate: "<<(exceptions/(float)(brick_columns*brick_rows))*100<<" %."<<endl;
        free(brick);
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
