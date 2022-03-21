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
    int paddle_width, paddle_height;
    int paddle_width_reverse;           // How big the area is where the ball goes back instead of forward
public:
    cPaddle(int posX, int posY, int width, int height)
    {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        paddle_width = width;
        paddle_height = height;
        paddle_width_reverse = paddle_width * 0.1;
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
    inline Vector2 getSize()
    {
        return {paddle_width, paddle_height};
    }
    inline int getPaddleWidthReverse()
    {
        return paddle_width_reverse;
    }
};

typedef struct Bricks
{
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
    int enabled;
} Bricks;

class cGameManager
{
private:
    int width,height;               // Screen width and height
    int level_width;
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
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool collision;                 // whether collision is on or not
    bool pause;

public:
    cGameManager()
    {
        //Open settings.txt file
        game_settings.open("..\\config\\settings.txt", ios::in);

        //Initialize all vars with their proper values
        brickCount = 0;
        quit = 0;
        movement_speed_paddle_base = 5;
        movement_speed_ball_base = 3;
        ball_size = 10;
        auto_move = 0;
        win = 0;
        pause = 0;

        //Load the settings file
        game_settings>>width>>height;
        game_settings>>fullscreen;
        cout<<"Width: "<<width<<endl<<"Height: "<<height<<"Fullscreen: "<<fullscreen;

        InitWindow(width,height,"Editor");
        SetTargetFPS(400);
        HideCursor();
        if(fullscreen == 1){
        ToggleFullscreen();}
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, ball_size);
        paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks)); // MemAlloc() is equivalent to malloc();

        //Load and resize image and then convert it to texture
        imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
        ImageResize(&imgBrick, 50, 35);
        texBrick = LoadTextureFromImage(imgBrick);

        //Rectangles that will be used for collision
        borderLeft = {0, 0, 10, GetScreenHeight()};
        borderRight = {GetScreenWidth() - 10, 0, GetScreenWidth(), GetScreenHeight()};
        borderTop = {0, 0, GetScreenWidth(), 10};
        borderBottom = {0, GetScreenHeight() - 10, GetScreenWidth(), 10};

    }

    void loadLevel()
    {
        loadLevelFile.open("..\\config\\level.txt", ios::in);
        int i = 0;
        brickCount = 0;
        while(!loadLevelFile.eof())
        {
            loadLevelFile>>level_width>>brick[i].position.x>>brick[i].position.y>>brick[i].brickWidth>>brick[i].brickHeight>>brick[i].type;
            if(width != level_width){
                int temp = width - level_width;
                if(temp > 0){
                    brick[i].position.x += temp / 2;
                }
                else {
                    brick[i].position.x -= temp / 2;
                }
            }
            brick[i].type++;
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
        for(int i = 0; i < brickCount - 1; i++)
        {
            if(brick[i].enabled)
            {
                if(brick[i].type == 1)
                {
                    bColor = SKYBLUE;
                }
                if(brick[i].type == 2)
                {
                    bColor = BLUE;
                }
                if(brick[i].type == 3)
                {
                    bColor = GRAY;
                }
                if(brick[i].type == 5)
                {
                    bColor = GOLD;
                }
                if(brick[i].type == 4)
                {
                    bColor = ORANGE;
                }
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
            }
        }
        //

        //For a bug(?) where the brick texture is spawned at 0,0 for some reason
        //DrawRectangle(0,0,50,50,BLACK);

        // Draw border
        DrawRectangleRec(borderTop,BROWN);
        DrawRectangleRec(borderBottom,RED);
        DrawRectangleRec(borderLeft,BROWN);
        DrawRectangleRec(borderRight,BROWN);

        // Paddle draw
        DrawRectangle(paddle->getX(),paddle->getY(),paddle->getSize().x,paddle->getSize().y,WHITE);
        //Dev
        DrawLine(paddle->getX() + paddle->getPaddleWidthReverse(),paddle->getY() - 5, paddle->getX() + paddle->getPaddleWidthReverse(), paddle->getY() + paddle->getSize().y + 5, RED);
        DrawLine(paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse(),paddle->getY() - 5, paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse(), paddle->getY() + paddle->getSize().y + 5, RED);

        //Ball draw
        DrawCircle(ball->getX(),ball->getY(),ball->getSize(),WHITE);
        EndDrawing();
    }

    void Input()
    {
        float frametime = GetFrameTime() * 100;
        int movement_speed_paddle = movement_speed_paddle_base * frametime;
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5)
        {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        //Mouse movement
        if(!auto_move && !pause)
        {
            if(paddle->getX() - movement_speed_paddle >= 10)
            {
                if(GetMouseDelta().x < 0 || IsKeyDown('A'))
                {
                    paddle->moveLeft(movement_speed_paddle + abs(GetMouseDelta().x));
                }
            }
            if(paddle->getX() + movement_speed_paddle + paddle->getSize().x <= GetScreenWidth() - 10)
            {
                if(GetMouseDelta().x > 0 || IsKeyDown('D'))
                {
                    paddle->moveRight(movement_speed_paddle + abs(GetMouseDelta().x));
                }
            }
        }
        //

        //Lock mouse position so it doesn't go outside of screen
        if(!pause)
        {
            SetMousePosition(GetScreenWidth() / 2,GetScreenHeight() / 2);
        }


        // Enable auto-moving
        if(IsKeyPressed('W'))
        {
            auto_move = !auto_move;
        }

        // Reset the level
        if(IsKeyPressed('R'))
        {
            reset();
        }

        //Pause the game
        if(IsKeyPressed('P'))
        {
            pause = !pause;
        }

        // Click to start the ball movement thing
        if(ball->getDirection() == STOP)
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                ball->changeDirection(UPRIGHT);
            }
        }
    }

    Vector2 BrickCollision(){
        //Brick Collision
        Vector2 ball_collision = {ball->getX(), ball->getY()};
        Rectangle brick_collision;
        for(int i = 0; i < brickCount - 1; i++)
        {
            brick_collision = {brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight};
            if(CheckCollisionCircleRec(ball_collision, ball->getSize(), brick_collision) && brick[i].enabled)
            {
                if(ball->getX() <= brick[i].position.x)
                {
                    if(ball->getDirection() == UPRIGHT)
                    {
                        ball->changeDirection(UPLEFT);
                    }
                    else
                    {
                        ball->changeDirection(DOWNLEFT);
                    }
                }
                else if(ball->getX() >= brick[i].position.x + brick[i].brickWidth)
                {
                    if(ball->getDirection() == UPLEFT)
                    {
                        ball->changeDirection(UPRIGHT);
                    }
                    else
                    {
                        ball->changeDirection(DOWNRIGHT);
                    }
                }
                else if(ball->getY() <= brick[i].position.y)
                {
                    if(ball->getDirection() == DOWNLEFT)
                    {
                        ball->changeDirection(UPLEFT);
                    }
                    else
                    {
                        ball->changeDirection(UPRIGHT);
                    }
                }
                else if(ball->getY() >= brick[i].position.y + brick[i].brickHeight)
                {
                    if(ball->getDirection() == UPRIGHT)
                    {
                        ball->changeDirection(DOWNRIGHT);
                    }
                    else
                    {
                        ball->changeDirection(DOWNLEFT);
                    }
                }
                if(brick[i].type == 4)
                {
                    brick[i].type = 0;
                }
                else if(brick[i].type != 5)       //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
                {
                    brick[i].type -= 1;
                }
                if(brick[i].type == 0)
                {
                    brick[i].enabled = 0;
                }
            }
        }
        return ball_collision;
    }

    void Logic()
    {
        Vector2 ball_collision = {ball->getX(), ball->getY()};
        float frametime = GetFrameTime() * 100;

        int movement_speed_paddle = movement_speed_paddle_base * frametime;
        if(movement_speed_paddle > movement_speed_paddle_base * 1.5)
        {
            movement_speed_paddle = movement_speed_paddle_base * 1.5;
        }

        //Auto-movement for the paddle (doesn't work well anymore since I added the ability to send the ball back in same direction)
        if(auto_move)
        {
            if(paddle->getX() - movement_speed_paddle > 10)
            {
                if(ball->getX() < paddle->getX() + 25)
                {
                    paddle->moveLeft(movement_speed_paddle);
                }
            }
            if(paddle->getX() + movement_speed_paddle + paddle->getSize().x < GetScreenWidth() - 10)
            {
                if(ball->getX() > paddle->getX() + 25)
                {
                    paddle->moveRight(movement_speed_paddle);
                }
            }
        }


        //Try to do ball movement based on frame-time (dunno if ti works)
        int movement_speed_ball = (movement_speed_ball_base + rand() % 6) * frametime;
        if(movement_speed_ball == 0)
        {
            movement_speed_ball = 1;
        }
        if(movement_speed_ball > movement_speed_ball_base * 2.5)
        {
            movement_speed_ball = movement_speed_ball_base * 2.5;
        }

        //Move only if game is NOT paused
        if(!pause)
        {
            ball->Move(movement_speed_ball);
        }

        //Left wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), borderLeft))
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

        //Right wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderRight))
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

        //Top wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderTop))
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

        //Bottom wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom))
        {
            cout<<"You Lose!"<<endl;
            reset();
        }


        //Paddle Collision
        ball_collision = {ball->getX(), ball->getY()};
        Rectangle paddle_collision = {paddle->getX(), paddle->getY(),paddle->getSize().x, paddle->getSize().y};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision))
        {
            if(paddle->getSize().x > 30)
            {
                if(ball->getX() < paddle->getX() + paddle->getPaddleWidthReverse())
                {
                    ball->changeDirection(UPLEFT);
                    cout<<"LEFT"<<endl;
                }
                if(ball->getX() > paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse())
                {
                    ball->changeDirection(UPRIGHT);
                    cout<<"RIGHT"<<endl;
                }
                if(ball->getX() >= paddle->getX() + paddle->getPaddleWidthReverse() && ball->getX() <= paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse())
                {
                    if(ball->getDirection() == DOWNLEFT)
                    {
                        ball->changeDirection(UPLEFT);
                    }
                    else
                    {
                        ball->changeDirection(UPRIGHT);
                    }
                    cout<<"MIDDLE"<<endl;
                }
            }
            else
            {
                if(ball->getDirection() == DOWNLEFT)
                {
                    ball->changeDirection(UPLEFT);
                }
                else ball->changeDirection(UPRIGHT);
            }
            while(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision))
            {
                if(!pause)
                {
                    Draw();
                    ball->Move(movement_speed_ball);
                    ball_collision = BrickCollision();
                }
            }
        }

        //Brick Collision
        ball_collision = BrickCollision();
    }

    void reset()
    {
        paddle->Reset();
        ball->Reset();
        loadLevel();
    }

    void checkWin()
    {
        int a = 0;
        for(int i = 0; i <= brickCount; i++)
        {
            a = a + brick[i].enabled;
        }
        a -= 1;
        if(a == 0)
        {
            //win = 1;
            cout<<"WIN!!!!"<<endl;
            reset();
        }
    }

    int Run()
    {
        loadLevel();
        while(!WindowShouldClose() & !win)
        {
            Input();
            Logic();
            checkWin();
            Draw();
        }
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
