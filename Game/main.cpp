#include <iostream>
#include <fstream>
#include "E:\WindowsFolders\Desktop\C++\Raylib Library\raylib\src\raylib.h"
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

class cBall {
  private:
    int x, y;
    int originalX, originalY;
    int current_size, originalSize;
    eDir direction;
  public:
    cBall(int posX, int posY, int ball_size) {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
        originalSize = ball_size;
        current_size = ball_size;
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
        direction = (eDir)((rand() % 4) + 1);
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
    void Move(int movement) {
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

typedef struct activePowerups {
    bool laser = 0;         // Gives you the ability to shoot lasers
    bool crosshair = 0;     // A crosshair pops over a random brick and breaks it in a short time after
    bool extend = 0;        // Extends the paddle
    bool pierce = 0;        // Gives the ball the ability to pierce all bricks
    bool fireball = 0;      // The ball becomes a fireaball. The ball explodes every time it hits a brick. Any brick in a certain radius also gets damaged
    bool nextLevel = 0;     // Instantly transports you to the next level
} activePowerups;

typedef struct powerup {
    activePowerups state;
    int type;
    Vector2 position;
    bool enabled;
} powerup;

class cPaddle {
  private:
    int x, y;
    int originalX, originalY;
    int paddle_width, paddle_height;
    int paddle_width_reverse;           // How big the area is where the ball goes back instead of forward
  public:
    cPaddle(int posX, int posY, int width, int height) {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        paddle_width = width;
        paddle_height = height;
        paddle_width_reverse = 13;
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
        return {paddle_width, paddle_height};
    }
    inline int getPaddleWidthReverse() {
        return paddle_width_reverse;
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
    int width,height;               // Screen width and height
    bool fullscreen;                // whether it's full-screen or not
    bool quit;                      // whether the game should quit or not
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

    int brickCount;
    int movement_speed_paddle_base; // base speed for paddle
    int movement_speed_ball_base;   // base speed for ball
    bool auto_move;                  // whether the paddle automatically moves or not. helps with testing simple stuff
    bool win;                       // whether you won or not. winning is when all bricks that can be destroyed are destroyed
    bool collision;                 // whether collision is on or not
    bool pause;

  public:
    cGameManager() {
        //Open settings.txt file
        game_settings.open("..\\config\\settings.txt", ios::in);
        //Initialize all vars with their proper values
        brickCount = 0;
        quit = 0;
        movement_speed_paddle_base = 8;
        movement_speed_ball_base = 3;
        auto_move = 0;
        win = 0;
        pause = 0;

        //Load the settings file
        game_settings>>width>>height>>fullscreen;
        //cout<<"Width: "<<width<<endl<<"Height: "<<height<<"Fullscreen: "<<fullscreen;

        InitWindow(width,height,"Editor");
        SetTargetFPS(400);
        HideCursor();
        if(fullscreen == 1) {
            ToggleFullscreen();
        }
        ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, 10);
        paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks)); // MemAlloc() is equivalent to malloc();
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


        //Rectangles that will be used for collision
        borderLeft = {0, 0, 10, GetScreenHeight()};
        borderRight = {GetScreenWidth() - 10, 0, GetScreenWidth(), GetScreenHeight()};
        borderTop = {0, 0, GetScreenWidth(), 10};
        borderBottom = {0, GetScreenHeight() - 10, GetScreenWidth(), 10};

    }

    void loadLevel() {
        loadLevelFile.open("..\\config\\level.txt", ios::in);
        int i = 0;
        float temp, temp2;
        float level_width, level_height;
        brickCount = 0;
        while(!loadLevelFile.eof()) {
            loadLevelFile>>level_width>>level_height>>brick[i].position.x>>brick[i].position.y>>brick[i].brickWidth>>brick[i].brickHeight>>brick[i].type;
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
            brick[i].enabled = 1;
            i++;
            brickCount++;
        }
        ImageResize(&imgBrick, brick[0].brickWidth, brick[0].brickHeight);
        texBrick = LoadTextureFromImage(imgBrick);
        loadLevelFile.close();
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
                        printf("Powerup enabled!\n");
                        c_powerup[i].enabled = 1;
                        changed = 1;
                        c_powerup[i].position.x = brick[brickIndex].position.x;
                        c_powerup[i].position.y = brick[brickIndex].position.y;
                    }
                    i++;
                } while(changed == 0 & i < 6);
            }
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(10,10);

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

        DrawRectangle(paddle->getX(), paddle->getY(), paddle->getPaddleWidthReverse(), paddle->getSize().y, YELLOW);
        DrawRectangle(paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse(), paddle->getY(), paddle->getPaddleWidthReverse(), paddle->getSize().y, YELLOW);

        Rectangle Source = {0,0,texPaddleEdge_L.width,texPaddleEdge_L.height};
        Rectangle Destination = {paddle->getX(), paddle->getY(), paddle->getPaddleWidthReverse(), paddle->getSize().y};
        DrawTexturePro(texPaddleEdge_L, Source, Destination, {0,0}, 0, WHITE);

        Source = {0,0,texPaddleEdge_R.width,texPaddleEdge_R.height};
        Destination = {paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse(), paddle->getY(), paddle->getPaddleWidthReverse(), paddle->getSize().y};
        DrawTexturePro(texPaddleEdge_R, Source, Destination, {0,0}, 0, WHITE);

        Source = {0,0,texPaddleBody.width,texPaddleBody.height};
        Destination = {paddle->getX() + paddle->getPaddleWidthReverse(), paddle->getY(), paddle->getSize().x -  2 * paddle->getPaddleWidthReverse(), paddle->getSize().y};
        DrawTexturePro(texPaddleBody, Source, Destination, {0,0}, 0, WHITE);

        //Ball draw
        DrawTexture(texBall, ball->getX() - ball->getSize(), ball->getY() - ball->getSize(), WHITE);
        EndDrawing();

        //Powerup / powerdown draw
        for(int i = 0; i < 6; i++) {
            if(c_powerup[i].enabled) {
                float frametime = GetFrameTime() * 100;
                int movement_speed_powerup = (movement_speed_ball_base + rand() % 6) * frametime;
                printf("Powerup drawn!\n");
                DrawTexture(texPowerup, c_powerup[i].position.x - 50, c_powerup[i].position.y - 25, WHITE);
                c_powerup[i].position.y += movement_speed_powerup;
            }
        }
    }

    void Input() {
        float frametime = GetFrameTime() * 100;
        int movement_speed_paddle = movement_speed_paddle_base * frametime;
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

        // Click to start the ball movement thing
        if(ball->getDirection() == STOP) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ball->changeDirection(UPRIGHT);
            }
        }
    }

    Vector2 BrickCollision() {
        //Brick Collision
        Vector2 ball_collision = {ball->getX(), ball->getY()};
        Rectangle brick_collision;
        for(int i = 0; i < brickCount - 1; i++) {
            brick_collision = {brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight};
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
            }
        }
        return ball_collision;
    }

    void Logic() {
        Vector2 ball_collision = {ball->getX(), ball->getY()};
        float frametime = GetFrameTime() * 100;

        int movement_speed_paddle = movement_speed_paddle_base * frametime;
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

        //Try to do ball movement based on frame-time (dunno if ti works)
        int movement_speed_ball = (movement_speed_ball_base + rand() % 6) * frametime;
        if(movement_speed_ball == 0) {
            movement_speed_ball = 1;
        }
        if(movement_speed_ball > movement_speed_ball_base * 2.5) {
            movement_speed_ball = movement_speed_ball_base * 2.5;
        }

        //Move only if game is NOT paused
        if(!pause) {
            ball->Move(movement_speed_ball);
        }

        //Left wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), borderLeft)) {
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getDirection() == DOWNLEFT) {
                ball->changeDirection(DOWNRIGHT);
            }
        }

        //Right wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderRight)) {
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getDirection() == DOWNRIGHT) {
                ball->changeDirection(DOWNLEFT);
            }
        }

        //Top wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderTop)) {
            if(ball->getDirection() == UPRIGHT) {
                ball->changeDirection(DOWNRIGHT);
            }
            if(ball->getDirection() == UPLEFT) {
                ball->changeDirection(DOWNLEFT);
            }
        }

        //Bottom wall collision
        ball_collision = {ball->getX(), ball->getY()};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom)) {
            //cout<<"You Lose!"<<endl;
            reset();
        }


        //Paddle Collision
        ball_collision = {ball->getX(), ball->getY()};
        Rectangle paddle_collision = {paddle->getX(), paddle->getY(),paddle->getSize().x, paddle->getSize().y};
        if(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision)) {
            if(paddle->getSize().x > 30) {
                if(ball->getX() < paddle->getX() + paddle->getPaddleWidthReverse()) {
                    ball->changeDirection(UPLEFT);
                    //cout<<"LEFT"<<endl;
                }
                if(ball->getX() > paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse()) {
                    ball->changeDirection(UPRIGHT);
                    //cout<<"RIGHT"<<endl;
                }
                if(ball->getX() >= paddle->getX() + paddle->getPaddleWidthReverse() && ball->getX() <= paddle->getX() + paddle->getSize().x - paddle->getPaddleWidthReverse()) {
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
                    ball->Move(movement_speed_ball);
                    ball_collision = BrickCollision();
                }
            }
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
        for(int i = 0; i < 6; i++){
            c_powerup[i].enabled = 0;
        }
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

    int Run() {
        loadLevel();
        while(!WindowShouldClose() & !win) {
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

int main() {
    SetRandomSeed(time(0));
    cGameManager cGame;
    int exit = cGame.Run();
    return exit;
}
