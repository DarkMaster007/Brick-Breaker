#include "cCollisionManager.h"

//Returns one if brick was destroyed. 0 if brick was not destroyed
static bool cCollisionManager::CheckCollision(cBricks &brick, cBall &ball){
        Rectangle brick_collision = {brick.position.x, brick.position.y, (float)brick.brickWidth, (float)brick.brickHeight};
        if(CheckCollisionCircleRec(Vector2{ball.getX(), ball.getY()}, ball.getSize(), brick_collision) && brick.enabled)
        {
            if(ball.getX() <= brick.position.x)
            {
                if(ball.getDirection() == UPRIGHT)
                {
                    ball.changeDirection(UPLEFT);
                }
                else
                {
                    ball.changeDirection(DOWNLEFT);
                }
            }
            else if(ball.getX() >= brick.position.x + brick.brickWidth)
            {
                if(ball.getDirection() == UPLEFT)
                {
                    ball.changeDirection(UPRIGHT);
                }
                else
                {
                    ball.changeDirection(DOWNRIGHT);
                }
            }
            else if(ball.getY() <= brick.position.y)
            {
                if(ball.getDirection() == DOWNLEFT)
                {
                    ball.changeDirection(UPLEFT);
                }
                else
                {
                    ball.changeDirection(UPRIGHT);
                }
            }
            else if(ball.getY() >= brick.position.y + brick.brickHeight)
            {
                if(ball.getDirection() == UPRIGHT)
                {
                    ball.changeDirection(DOWNRIGHT);
                }
                else
                {
                    ball.changeDirection(DOWNLEFT);
                }
            }
            if(brick.type == 4)
            {
                brick.type = 0;
            }
            else if(brick.type != 5)       //1 - Normal, 2 - 2HP, 3 - 3HP, 4 - Explosive, 5 - Gold(Unbreakable)
            {
                brick.type -= 1;
            }
            if(brick.type == 0)
            {
                brick.enabled = 0;
                return 0;
            }
            ball.randomizeMovement();
            return 1;
//            SpawnPowerup(i);
//            PlaySound(soundBounceGeneral);   add these 2 in GameManager after checking collision.
        }
}
static bool cCollisionManager::CheckCollision(cPaddle &paddle, cBall &ball){
            if(ball.getX() < paddle.getX() + paddle.getBounceReverseArea())
            {
                ball.changeDirection(UPLEFT);
            }
            if(ball.getX() > paddle.getX() + paddle.getSize().x - paddle.getBounceReverseArea())
            {
                ball.changeDirection(UPRIGHT);
            }
            if(ball.getX() >= paddle.getX() + paddle.getBounceReverseArea() && ball.getX() <= paddle.getX() + paddle.getSize().x - paddle.getBounceReverseArea())
            {
                if(ball.getDirection() == DOWNLEFT)
                {
                    ball.changeDirection(UPLEFT);
                }
                else
                {
                    ball.changeDirection(UPRIGHT);
                }
            }
//        while(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision))
//        {
//            if(!pause)
//            {
//                Draw();
//                ball->Move();
//                ball_collision = BallToRecCollision();
//            }
//        }
}
static bool cCollisionManager::WallCollision(float wallThickness){

}
