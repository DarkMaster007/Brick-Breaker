#include "cGameManager.h"

cGameManager::cGameManager()
{
    SetRandomSeed(time(NULL));
    //Open settings.txt file
    fp = fopen("..\\config\\settings.txt", "r");
    //Initialize all vars with their proper values
    brickCount = 0;
    quit = 0;
    auto_move = 0;
    win = 0;
    pause = 0;

    //Load the settings file
    fscanf(fp, "%d %d %d", &width, &height, (int*)&fullscreen);
    //printf("Width: %d\nHeight: %d\nFullscreen: %d\n", width, height, fullscreen);

    fclose(fp);

    InitWindow(width,height,"Brick Breaker");
    InitAudioDevice();
    //SetTargetFPS(60);
    HideCursor();
    if(fullscreen == 1)
    {
        ToggleFullscreen();
    }
    ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, 10);
    paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
    brick = (cBricks *)MemAlloc(MAX_BRICKS*sizeof(cBricks));  //MemAlloc() is equivalent to malloc();
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

void cGameManager::loadLevel()
{
    fp = fopen("..\\config\\level.txt", "r");
    int i = 0;
    int temp, temp2;
    int level_width, level_height;
    brickCount = 0;
    while(!feof(fp))
    {
        fscanf(fp, "%d %d %f %f %d %d %d", &level_width, &level_height, &brick[i].position.x, &brick[i].position.y, &brick[i].brickWidth, &brick[i].brickHeight, &brick[i].type);
        if(width != level_width || height != level_height)
        {
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
        if(brick[i].position.x < 10)
        {
            brick[i].position.x += 50;
            if(brick[i].position.x < 10)
            {
                brick[i].position.x = 11;
            }
        }
        if(brick[i].position.x + brick[i].brickWidth > width - 10)
        {
            brick[i].position.x -= 50;
            if(brick[i].position.x < 10)
            {
                brick[i].position.x = width - 11 - brick[i].brickWidth;
            }
        }
        //
        //Y clamping
        if(brick[i].position.y + brick[i].brickHeight > GetScreenHeight() - 350)
        {
            brick[i].position.y -= 25;
            if(brick[i].position.y > GetScreenHeight() - 350)
            {
                brick[i].position.y = GetScreenHeight() - 351 - brick[i].brickHeight;
            }
        }
        if(brick[i].position.y < 0)
        {
            brick[i].position.y += 50;
            if(brick[i].position.y < 0)
            {
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

void cGameManager::SpawnPowerup(int brickIndex)
{
    int change_percent = 35;        // change this to change chance; I think that's how this works right?
    int generated_num = GetRandomValue(0, 1000);
    if(generated_num <= change_percent * 10)
    {
        if(!brick[brickIndex].enabled)
        {
            int i = 0;
            bool changed = 0;
            do
            {
                if(c_powerup[i].enabled == 0)
                {
                    printf("Powerup enabled!");
                    c_powerup[i].enabled = 1;
                    c_powerup[i].type = (eActivePowerups)GetRandomValue(0, 5);      // TODO (DarkMaster#7#10/13/22): Implement and actual method to get powerups. Next level should be low chance.
                    printf(" Type: %i\n", c_powerup[i].type);
                    changed = 1;                    // TODO (DarkMaster#4#10/12/22): To change, I don't think "changed" does anything
                    c_powerup[i].position.x = brick[brickIndex].position.x;
                    c_powerup[i].position.y = brick[brickIndex].position.y;
                }
                i++;
            }
            while(changed == 0 && i < 6);
        }
    }
}

void cGameManager::Draw()
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
    for(int i = 0; i < 6; i++)
    {
        if(c_powerup[i].enabled)
        {
            int movement_speed_powerup = 288;
            movement_speed_powerup = movement_speed_powerup * GetFrameTime();
            //printf("Powerup drawn!\n");
            DrawTexture(texPowerup, c_powerup[i].position.x - 50, c_powerup[i].position.y - 25, WHITE);
            c_powerup[i].position.y += movement_speed_powerup;
        }
    }

    //Debug stuff here:
    DrawFPS(10,10);
    float temp = GetFrameTime();
    GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, TextFormat("%f", std::abs(temp)));

    EndDrawing();
}

void cGameManager::Input()
{
    //Mouse movement
    if(!pause)
    {
        if(paddle->getX() - texPaddleEdge_L.width >= 10)
        {
            if(paddle->getX() + paddle->getSize().x + texPaddleEdge_R.width <= GetScreenWidth() - 10)
            {
                if(!auto_move)
                {
                    paddle->Move();
                }
                else paddle->setX(ball->getX() - paddle->getSize().x / 2);
            }
            else paddle->setX(GetScreenWidth() - paddle->getSize().x - 20);
        }
        else paddle->setX(20);
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

    if(IsKeyPressed('F'))
    {
        ToggleFullscreen();
    }

    if(IsKeyPressed('M'))
    {
        soundMute = !soundMute;
        SetMasterVolume(soundVolume * !soundMute);
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

Vector2 cGameManager::BallToRecCollision()
{
    //Brick Collision
    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};
    Rectangle brick_collision;
    for(int i = 0; i < brickCount - 1; i++)
    {
        brick_collision = {brick[i].position.x, brick[i].position.y, (float)brick[i].brickWidth, (float)brick[i].brickHeight};
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
            SpawnPowerup(i);
            PlaySound(soundBounceGeneral);
            ball->randomizeMovement();
        }
    }
    return ball_collision;
}

void cGameManager::Logic()
{
    //Move only if game is NOT paused
    if(!pause)
    {
        ball->Move();
    }


    // Check collisions between bricks and balls
    for (int j = 0; j < cBricks::instanceCount; j++) {
        for (int k = 0; k < cBall::instanceCount; k++) {
            if(cCollisionManager::CheckCollision(brick[j], ball[k]))
            {PlaySound(soundBounceGeneral);}
        }
    }

    /*
    // Check collisions between paddles and balls
    for (int i = 0; i < cPaddle::instanceCount; i++) {
        for (int k = 0; k < cBall::instanceCount; k++) {
            if(cCollisionManager::CheckCollision(paddle[i], ball[k]))
            {PlaySound(soundBouncePaddle);}
        }
    }*/


    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};

    //Left wall collision
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
        PlaySound(soundBounceGeneral);
    }else

    //Right wall collision
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
        PlaySound(soundBounceGeneral);
    }else

    //Top wall collision
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
        PlaySound(soundBounceGeneral);
    }else

    //Bottom wall collision
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom))
    {
        //cout<<"You Lose!"<<endl;
        PlaySound(soundDeath);
        reset();
    }

    for(int i = 0; i < 6; i++)
    {
        if(c_powerup[i].position.y > GetScreenHeight() - 10)
        {
            c_powerup[i].enabled = 0;
        }
    }
}

void cGameManager::reset()
{
    paddle->Reset();
    ball->Reset();
    loadLevel();
    for(int i = 0; i < 6; i++)
    {
        c_powerup[i].enabled = 0;
    }
    SetRandomSeed(time(NULL));
}

void cGameManager::checkWin()
{
    int a = 0;
    for(int i = 0; i <= brickCount; i++)
    {
        if(brick[i].type != 5)
        {
            a = a + brick[i].enabled;
        }
    }
    a -= 1;
    if(a == 0)
    {
        //win = 1;
        //cout<<"WIN!!!!"<<endl;
        reset();
    }
}

void cGameManager::cleanup()
{
    free(brick);
    free(c_powerup);
    delete[] ball;
    delete[] paddle;
    _fcloseall();
    CloseWindow();
}

int cGameManager::Run()
{
    loadLevel();
    while(!WindowShouldClose() & !win)
    {
        Input();
        Logic();
        checkWin();
        Draw();
    }
    cleanup();
    return 0;
}
