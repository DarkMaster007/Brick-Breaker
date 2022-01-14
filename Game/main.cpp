#include <iostream>
#include <fstream>
#include "include/raylib.h"
#include <string.h>
using std::fstream;
using std::endl;
using std::string;
using std::cout;
using std::ios;

fstream loadLevelFile;
fstream game_settings;
enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};

class cBall
{
private:
    int x, y;
    int originalX, originalY;
    int current_size, originalSize;
    eDir direction;
public:
    float frametime;
    float movement;
    cBall(int posX, int posY, int ball_size, float frame = 1.0)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
        originalSize = ball_size;
        current_size = ball_size;
        frametime = frame;
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
    inline int setFrameTime(int frame){
    frametime = frame;
    }
    void Move()
    {
        movement = (3 + rand() % 4) * frametime;
        if(movement < 1) movement = 1;
        switch (direction)
        {
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
    inline int moveLeft(int pixels)
    {
        x -= pixels;
    }
    inline int moveRight(int pixels)
    {
        x += pixels;
    }
};

class cGameManager
{
private:
    int width,height;
    int offset;                     // distance between bricks / 2 (don't ask); loaded from settings.txt
    int brick [100][100];           // max amount of bricks; example: brick[100] [100] is 100 x 100 bricks = 10000
    int brick_width, brick_height;  // width and height of individual bricks; loaded from settings.txt
    int brick_columns, brick_rows;  // how many rows and how many columns of bricks there should be; calculated
    string brick_color;             // the bricks color; loaded from settings.txt
    bool fullscreen;                // whether it's full-screen or not
    int brick_centering;            // amount of pixels to offset bricks to center them (looks nice :) ); calculated
    bool quit;
    cBall *ball;
    cPaddle *paddle;
    Rectangle borderLeft;
    Rectangle borderRight;
    Rectangle borderTop;
    Rectangle borderBottom;
    Rectangle recPaddle;
    int movement_speed;
    int ball_size;
    int auto_move;
    int exceptions;
public:
    cGameManager()
    {
        game_settings.open("..//config//settings.txt", ios::in);

        quit = 0;
        offset = 10;
        movement_speed = 10;
        ball_size = 10;
        auto_move = 1;
        exceptions = 0;

        game_settings>>width>>height;
        game_settings>>brick_width>>brick_height;
        game_settings>>fullscreen;
        game_settings>>brick_color;

        InitWindow(width,height,"Editor");
        SetTargetFPS(480);
        HideCursor();
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, ball_size);
        paddle = new cPaddle(GetScreenWidth() / 2, GetScreenHeight() - 35);
        ball->changeDirection(UPRIGHT);
        ball->frametime = 0.1;

        //calculations
        brick_columns = GetScreenWidth() / (brick_width + offset + 4);  // amount of bricks length
        brick_rows = (GetScreenHeight() / 2) / (brick_height + offset); // amount of bricks height
        brick_centering = (GetScreenWidth() - brick_columns * brick_width - brick_columns * offset + offset) / 4;
        //brick_centering = 0;

        //Rectangles
        borderLeft = {0, 0, offset, GetScreenHeight()};
        borderRight = {GetScreenWidth() - offset, 0, GetScreenWidth(), GetScreenHeight()};
        borderTop = {0, 0, GetScreenWidth(), offset};
        borderBottom = {0, GetScreenHeight() - offset, GetScreenWidth(), offset};

        //cout's for testing; dev shit, remove from final
        cout<<"Brick width: "<<brick_width<<endl;
        cout<<"Brick height: "<<brick_height<<endl;
        cout<<"Full-screen: "<<fullscreen<<endl;
        cout<<"Color value: "<<brick_color<<endl;
        cout<<"Value to center bricks with: "<<brick_centering<<endl;
        cout<<"Brick columns: "<<brick_columns<<endl<<"Brick rows: "<<brick_rows<<endl;
        cout<<"Bricks total: "<<brick_columns*brick_rows<<endl;

    }
    void loadLevel()
    {
        loadLevelFile.open("..//config//level.txt", ios::in);
        int file;
        for(int j = 0; j < brick_rows; j++)
        {
            for(int i = 0; i < brick_columns; i++)
            {
                loadLevelFile>>file;
                brick[i][j] = file;
            }
        }
    }

    void CheckPattern()  //output the current brick layout to...well...check it; dev shit, remove from final
    {
        cout<<endl;
        for(int j = 0; j < brick_rows; j++)
        {
            for(int i = 0; i < brick_columns; i++)
            {
                cout<<brick[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    Color colors()  //brick colors; uses string from settings.txt
    {
        if(brick_color == "RED")
        {
            return RED;
        }
        if(brick_color == "BLUE")
        {
            return BLUE;
        }
        if(brick_color == "GREEN")
        {
            return GREEN;
        }
        if(brick_color == "YELLOW")
        {
            return YELLOW;
        }
        else
        {
            return WHITE;
        }
        return Color{GetRandomValue(10,255),GetRandomValue(10,255),GetRandomValue(10,255),GetRandomValue(150,255)};
    }

    void Draw()
    {
        ClearBackground(BLACK);
        BeginDrawing();
        //Loop to draw bricks
        for(int i = 0; i < brick_columns; i++)
        {
            for(int j = 0; j < brick_rows; j++)
            {
                if(brick[i][j])
                {
                    DrawRectangle(brick_centering + (offset * 2) + i * brick_width + i * offset, offset * 2 + j * brick_height + j * offset, brick_width, brick_height,colors());
                }
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
        /*line(ballX - 15,ballY,ballX + 15,ballY);
        outtextxy(ballX + 25, ballY - 10,"Y");
        line(ballX,ballY - 15,ballX,ballY + 15);
        outtextxy(ballX, ballY + 25,"X");*/
        EndDrawing();
    }

    void Input()
    {
        if(paddle->getX() - movement_speed - 50 > offset)
        {
            if(paddle->getX() > GetMouseX())
                {
                    paddle->moveLeft(movement_speed);
                }
            if(IsKeyDown('A'))
            {
                paddle->moveLeft(movement_speed);
            }
        }
        if(paddle->getX() + movement_speed + 50 < GetScreenWidth() - offset)
        {
            if(paddle->getX() < GetMouseX())
                {
                    paddle->moveRight(movement_speed);
                }
            if(IsKeyDown('D'))
            {
                paddle->moveRight(movement_speed);
            }
        }
        if(IsKeyPressed('W'))
        {
            auto_move = 1;
        }
        if(IsKeyPressed('S'))
        {
            //
        }
    }

    void Logic()
    {
        if(auto_move == 1)
        {
            //
        }

        ball->setFrameTime(GetFrameTime());
        ball->Move();
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), borderLeft)) //left wall
        {
            if(ball->getDirection() == UPLEFT)
            {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getDirection() == DOWNLEFT)
            {
                ball->changeDirection(DOWNRIGHT);
            }
        }
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(),borderRight))
        {
            if(ball->getDirection() == UPRIGHT)
            {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT)
            {
                ball->changeDirection(DOWNLEFT);
            }
        }
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(),borderTop))
        {
            if(ball->getDirection() == UPRIGHT)
            {
                ball->changeDirection(DOWNRIGHT);
            }
            if(ball->getDirection() == UPLEFT)
            {
                ball->changeDirection(DOWNLEFT);
            }
        }

        //Paddle Collision
        if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), {paddle->getX() - 50, paddle->getY(),100, 10}))
        {
            if(ball->getDirection() == DOWNLEFT)
            {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT)
            {
                ball->changeDirection(UPRIGHT);
            }
        }

        //Collision for bricks
        for(int i = 0; i < brick_columns; i++)
        {
            for(int j = 0; j < brick_rows; j++)
            {
                if(brick[i][j])
                {
                    int x = brick_centering + (offset * 2) + i * brick_width + i * offset;
                    int y = offset * 2 + j * brick_height + j * offset;
                    if(CheckCollisionCircleRec({ball->getX(), ball->getY()},ball->getSize(), {x, y, brick_width, brick_height}))
                    {
                        brick[i][j] = 0;

                        if(ball->getX() < x + 5 && ball->getY() > y && ball->getY() < (y + brick_height))  //Left brick side collision
                        {
                            if(ball->getDirection() == UPRIGHT)
                            {
                                ball->changeDirection(UPLEFT);
                            }
                            else
                            {
                                if(ball->getDirection() == DOWNRIGHT)
                                {
                                    ball->changeDirection(DOWNLEFT);
                                }
                            }
                        }
                        else if(ball->getX() > (x + brick_width) && ball->getY() > y && ball->getY() < (y + brick_height)) //Right brick side collision
                        {
                            if(ball->getDirection() == UPLEFT)
                            {
                                ball->changeDirection(UPRIGHT);
                            }
                            else
                            {
                                if(ball->getDirection() == DOWNLEFT)
                                {
                                    ball->changeDirection(DOWNRIGHT);
                                }
                            }
                        }
                        else if(ball->getY() < y && ball->getX() > x && ball->getX() < (x + brick_width))   //Top brick side collision
                        {
                            if(ball->getDirection() == DOWNLEFT)
                            {
                                ball->changeDirection(UPLEFT);
                            }
                            if(ball->getDirection() == DOWNRIGHT)
                            {
                                ball->changeDirection(UPRIGHT);
                            }
                        }
                        else if(ball->getY() > (y + brick_height) && ball->getX() > x && ball->getX() < (x + brick_width))   //Bottom brick side collision
                        {
                            if(ball->getDirection() == UPLEFT)
                            {
                                ball->changeDirection(DOWNLEFT);
                            }
                            if(ball->getDirection() == UPRIGHT)
                            {
                                ball->changeDirection(DOWNRIGHT);
                            }
                        }
                        else {ball->randomDirection();
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

    int Run()
    {
        loadLevel();
        CheckPattern();
        while(!WindowShouldClose())
        {
            if(IsWindowFocused())
            {
                Input();
                Logic();
            }
            Draw();
        }
        cout<<"Exception rate: "<<(exceptions/(float)(brick_columns*brick_rows))*100<<" %."<<endl;
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
