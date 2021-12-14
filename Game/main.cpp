#include <iostream>
#include <fstream>
#include "include/raylib.h"
#include <string.h>
using namespace std;

//ifstream lvl_builder("..//Game//config//level.txt");
//ifstream settings("..//Game//config//settings.txt");
enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};

//class cBall
//{
//private:
//    int x, y;
//    int originalX, originalY;
//    eDir direction;
//public:
//    cBall(int posX, int posY)
//    {
//        originalX = posX;
//        originalY = posY;
//        x = posX;
//        y = posY;
//        direction = STOP;
//    }
//    void Reset()
//    {
//        x = originalX;
//        y = originalY;
//        direction = STOP;
//    }
//    void changeDirection(eDir d)
//    {
//        direction = d;
//    }
//    void randomDirection()
//    {
//        direction = (eDir)((rand() % 4) + 1);
//    }
//    inline int getX()
//    {
//        return x;
//    }
//    inline int getY()
//    {
//        return y;
//    }
//    inline eDir getDirection()
//    {
//        return direction;
//    }
//    void Move()
//    {
//        switch (direction)
//        {
//        case STOP:
//            break;
//        case UPLEFT:
//            x-=3;
//            y-=3;
//            break;
//        case DOWNLEFT:
//            x-=3;
//            y+=3;
//            break;
//        case UPRIGHT:
//            x+=3;
//            y-=3;
//            break;
//        case DOWNRIGHT:
//            x+=3;
//            y+=3;
//            break;
//        default:
//            break;
//        }
//    }
//};
//class cPaddle
//{
//private:
//    int x, y;
//    int originalX, originalY;
//public:
//    cPaddle(int posX, int posY)
//    {
//        originalX = posX;
//        originalY = posY;
//        x = posX;
//        y = posY;
//    }
//    inline int getX()
//    {
//        return x;
//    }
//    inline int getY()
//    {
//        return y;
//    }
//    void Reset()
//    {
//        x = originalX;
//        y = originalY;
//    }
//    inline int moveLeft()
//    {
//        x -= 20;
//    }
//    inline int moveRight()
//    {
//        x += 20;
//    }
//};

int main(){
    InitWindow(650,480,"Brick Breaker");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        BeginDrawing();
        DrawRectangle(650/2-50,480/2-20,50,20,WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 1;
}
