#include "cGameManager.h"

double cGameManager::startTimer = 0;

cGameManager::cGameManager()
{
    SetRandomSeed(time(NULL));
    //Open settings.txt file
    fp = fopen("..\\config\\settings.txt", "r");
    //Initialize all vars with their proper values
    quit = 0;
    auto_move = 0;
    win = 0;
    pause = 0;

    //Load the settings file
    fscanf(fp, "%d %d %d", &width, &height, (int*)&fullscreen);
#ifdef _DEBUG
    printf("Width: %d\nHeight: %d\nFullscreen: %d\n", width, height, fullscreen);
#endif // _DEBUG

    fclose(fp);

    InitWindow(width,height,"Editor");
    InitAudioDevice();
    SetTargetFPS(120);
    HideCursor();
    if(fullscreen == 1)
    {
        ToggleFullscreen();
    }
    ball = new cBall(GetScreenWidth() / 2,GetScreenHeight() - 50, 10);
    paddle = new cPaddle(GetScreenWidth() / 2 - 50, GetScreenHeight() - 35, 100, 10);
    brick = (cBricks *)MemAlloc(MAX_BRICKS*sizeof(cBricks));  //MemAlloc() is equivalent to malloc();
    c_powerup = (cPowerup *)MemAlloc(6*sizeof(cPowerup));

    //Load and resize image and then convert it to texture
    texPaddleEdge_L = LoadTexture("..//resources//Breakout-Paddle_L.png");
    texPaddleEdge_R = LoadTexture("..//resources//Breakout-Paddle_R.png");
    texPaddleBody = LoadTexture("..//resources//Breakout-Paddle_middle.png");
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
    Image imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
    ImageResize(&imgBrick, 100*10, 70*10);
    Texture2D texBrick = LoadTextureFromImage(imgBrick);
    fp = fopen("..\\config\\level.txt", "r");
    int i = 0;
    int tempWidth, tempHeight;
    int levelWidth, levelHeight;
    float brickPosX, brickPosY;
    int brickWidth, brickHeight, brickType;
    cBricks::brickCount = 0;

    while(!feof(fp))
    {
        fscanf(fp, "%d %d %f %f %d %d %d", &levelWidth, &levelHeight, &brickPosX, &brickPosY, &brickWidth, &brickHeight, &brickType);
        if(width != levelWidth || height != levelHeight)
        {
            tempWidth = width / levelWidth;
            tempHeight = height / levelHeight;
            brickPosX *= tempWidth;
            brickPosY *= tempHeight;
            brickWidth *= (tempWidth*1.5);
            brickHeight *= (tempHeight*1.2);
#ifdef _DEBUG
            printf("X mod: %d/nY mod: %d/n",tempWidth,tempHeight);
            printf("Level res to game res ratio is: %d to %d./n",tempWidth,tempHeight);
#endif // _DEBUG
        }
        //X clamping
        if(brickPosX < 10)
        {
            brickPosX += 50;
            if(brickPosX < 10)
            {
                brickPosX = 11;
            }
        }
        if(brickPosX + brickWidth > width - 10)
        {
            brickPosX -= 50;
            if(brickPosX < 10)
            {
                brickPosX = width - 11 - brickWidth;
            }
        }
        //
        //Y clamping
        if(brickPosY + brickHeight > GetScreenHeight() - 350)
        {
            brickPosY -= 25;
            if(brickPosY > GetScreenHeight() - 350)
            {
                brickPosY = GetScreenHeight() - 351 - brickHeight;
            }
        }
        if(brickPosY < 0)
        {
            brickPosY += 50;
            if(brickPosY < 0)
            {
                brickPosY = 11;
            }
        }
        //
        new (&brick[i]) cBricks(brickPosX, brickPosY, brickWidth, brickHeight, brickType, texBrick);
        i++;
        cBricks::brickCount++;
    }
    fclose(fp);
}

void cGameManager::SpawnPowerup(int brickIndex)
{
    if(!brick[brickIndex].enabled)
    {
        int i, change = 0;
        for(i = 0; i < 6; i++)
        {
            if(!c_powerup[i].getEnabled())
            {
                break;
            }
            i++;
        }
        c_powerup[i].spawnPowerup(brick[brickIndex]);
    }
}

void cGameManager::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    //Loop to draw bricks
    for(int i = 0; i < cBricks::brickCount - 1; i++)
    {
        if(brick[i].enabled)
        {
            DrawTexturePro(cBricks::texture, Rectangle{0, 0, cBricks::texture.width, cBricks::texture.height}, Rectangle{brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight},Vector2{0,0}, 0.0f, brick[i].getColor());

            //Dev stuff:
#ifdef _DEBUG
            DrawText(TextFormat("%d", i+1), brick[i].position.x + brick[i].brickWidth / 2, brick[i].position.y + brick[i].brickHeight / 2, 5, RED);
            DrawText(TextFormat("%d", i+1), brick[i].position.x + brick[i].brickWidth / 2, brick[i].position.y + brick[i].brickHeight / 2, 5, RED);
#endif // _DEBUG
        }
    }
    //

    // Draw border
    DrawRectangleRec(borderTop,BROWN);
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
    DrawTexture(cBall::texBall, ball->getX() - ball->getSize(), ball->getY() - ball->getSize(), WHITE);

    //Powerup / powerdown draw
    for(int i = 0; i < 6; i++)
    {
        if(c_powerup[i].getEnabled())
        {
            int movement_speed_powerup = GetRandomValue(300, 400) * GetFrameTime();
            DrawTexture(texPowerup, c_powerup[i].getPosition().x - 50, c_powerup[i].getPosition().y - 25, WHITE);
            c_powerup[i].setPosition(Vector2{c_powerup[i].getPosition().x,c_powerup[i].getPosition().y + movement_speed_powerup});
        }
    }

    //Debug stuff here:
#ifdef _DEBUG
    DrawRectangleRec(borderBottom,RED);
    DrawFPS(10,10);
    if(ball->getDirection() != STOP)
        GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, TextFormat("%f", GetTime() - startTimer));
    else
        GuiStatusBar({0, (float)GetScreenHeight() - 30, 100, 30}, "0.00000");
#endif // _DEBUG

    EndDrawing();
}

void cGameManager::Input()
{
    //Mouse movement
    if(!auto_move && !pause)
    {
        if(paddle->getX() - paddle->getSpeed() >= 10)
        {
            if(paddle->getX() + paddle->getSize().x + paddle->getSpeed() <= GetScreenWidth() - 10)
            {
                if(GetMouseDelta().x < 0 || IsKeyDown('A'))
                {
                    paddle->moveLeft();
                }
                if(GetMouseDelta().x > 0 || IsKeyDown('D'))
                {
                    paddle->moveRight();
                }
            }
            else paddle->moveLeft(1);
        }
        else paddle->moveRight(1);
    }
    //

    //Lock mouse position so it doesn't go outside of screen
    if(!pause)
    {
        SetMousePosition(GetScreenWidth() / 2,GetScreenHeight() / 2);
    }


    // Enable auto-moving
#ifdef _DEBUG
    if(IsKeyPressed('W'))
    {
        auto_move = !auto_move;
    }
#endif // _DEBUG

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
}

Vector2 cGameManager::BrickCollision()
{
    //Brick Collision
    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};
    Rectangle brick_collision;
    for(int i = 0; i < cBricks::brickCount - 1; i++)
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
    // Click to start the ball movement thing
    if(ball->getDirection() == STOP)
    {
        startTimer = GetTime();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            ball->changeDirection(UPRIGHT);
        }
    }

    //Make bricks fall after 60 seconds
    if(ball->getDirection() != STOP && !pause)
    {
        if(GetTime() - startTimer > 60.0f)
        {
            if(fmod(GetTime() - startTimer,10.0) < 0.05)
            {
                for(int i = 0; i < cBricks::brickCount; i++)
                {
                    brick[i].position.y += 5;
                }
            }
        }
    }

    Vector2 ball_collision = {(float)ball->getX(), (float)ball->getY()};

    //Auto-movement for the paddle (doesn't work well anymore since I added the ability to send the ball back in same direction)
    if(auto_move)
    {
        if(paddle->getX() - paddle->getSpeed() > 10)
        {
            if(ball->getX() < paddle->getX() + 25)
            {
                paddle->moveLeft();
            }
        }
        if(paddle->getX() + paddle->getSize().x + paddle->getSpeed() < GetScreenWidth() - 10)
        {
            if(ball->getX() > paddle->getX() + 25)
            {
                paddle->moveRight();
            }
        }
    }

    //Move only if game is NOT paused
    if(!pause)
    {
        ball->Move();
    }

    //Left wall collision
    ball_collision = {(float)ball->getX(), (float)ball->getY()};
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
    }

    //Right wall collision
    ball_collision = {(float)ball->getX(), (float)ball->getY()};
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
    }

    //Top wall collision
    ball_collision = {(float)ball->getX(), (float)ball->getY()};
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
    }

    //Bottom wall collision
    ball_collision = {(float)ball->getX(), (float)ball->getY()};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(),borderBottom))
    {
        PlaySound(soundDeath);
        reset();
    }


    //Paddle Collision
    ball_collision = {(float)ball->getX(), (float)ball->getY()};
    Rectangle paddle_collision = {(float)paddle->getX(), (float)paddle->getY(), paddle->getSize().x, paddle->getSize().y};
    if(CheckCollisionCircleRec(ball_collision,ball->getSize(), paddle_collision))
    {
        if(paddle->getSize().x > 30)
        {
            if(ball->getX() < paddle->getX() + paddle->getBounceReverseArea())
            {
                ball->changeDirection(UPLEFT);
            }
            if(ball->getX() > paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea())
            {
                ball->changeDirection(UPRIGHT);
            }
            if(ball->getX() >= paddle->getX() + paddle->getBounceReverseArea() && ball->getX() <= paddle->getX() + paddle->getSize().x - paddle->getBounceReverseArea())
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
                ball->Move();
                ball_collision = BrickCollision();
            }
        }
        PlaySound(soundBouncePaddle);
    }

    //Brick Collision
    ball_collision = BrickCollision();

    for(int i = 0; i < 6; i++)
    {
        if(c_powerup[i].getPosition().y > GetScreenHeight() - 10)
        {
            c_powerup[i].setEnabled(0);
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
        c_powerup[i].setEnabled(0);
    }
    SetRandomSeed(time(NULL));
}

void cGameManager::checkWin()
{
    int a = 0;
    for(int i = 0; i <= cBricks::brickCount; i++)
    {
        if(brick[i].type != 5)
        {
            a = a + brick[i].enabled;
        }
    }
    a -= 1;
    if(a == 0)
    {
        reset();
    }
}

cGameManager::~cGameManager()
{
    for(int i = 0; i < cBricks::brickCount; i++)
    {
        brick[i].~cBricks();
    }
    MemFree(brick);
    MemFree(c_powerup);
    delete[] ball;
    delete[] paddle;
    _fcloseall();
    CloseWindow();
    printf("Destructor finished.\n");
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
    return 0;
}
