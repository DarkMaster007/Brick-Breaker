// TODO (Dark#1#):  Allow the player to load the existing level
// TODO (Dark#1#):  Allow the player to not replace existing level file
// NOTE (Dark#1#): Allow to resize the bricks at will (maybe?)
// TODO (Dark#1#): Consolidate if's into either switch's of less if's
#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include <raygui.h>
#include "../Release/resources/includes/soundSelect.h"

#define MAX_INPUT_CHARS   9
#define MAX_INPUT_INT     4
#define MAX_BRICKS        300

class cSettings
{
private:
    FILE *fp;
    Font font;
    Texture2D title;
    int export_width, export_height, export_brick_width, export_brick_height, export_fullscreen, export_offset;
    int current_screen;
    int global_width, global_height;
    int fullscreen;
    Rectangle button_settings;
    Rectangle button_extra;
    Rectangle button_start;
    int quit;
    bool update;
    int fail_open;

    Wave waveSelect;

    Rectangle screenWidthBox;   //the box to write screen width to in settings
    int settings_screenWidth;
    bool spinnerWidthEnable;

    Rectangle screenHeightBox;  //the box to write screen height to in settings
    int settings_screenHeight;
    bool spinnerHeightEnable;

public:
    cSettings(int width, int height, int fullscreen_loaded)
    {
        fp = NULL;
        font = LoadFont("fonts/rainyhearts16.ttf");
        GuiSetFont(font);
        global_width = width;
        global_height = height;
        settings_screenWidth = global_width;
        settings_screenHeight = global_height;
        fullscreen = fullscreen_loaded;
        quit = 0;
        current_screen = 0; // 0 - Menu, 1 - Settings, 2 - Extra, 3 - Start
        update = 0;
        fail_open = 0;
        fp = fopen("..//config//settings.txt", "r+");
        if(fp == NULL)
        {
            fail_open = 1;
        }

        //Main screen
        int x = global_width / 2 - 90;
        int y = global_height * 0.35;
        int box_width = 180;
        int box_height = 60;
        button_settings = { (float)x, (float)y, (float)box_width, (float)box_height };
        button_extra = { (float)x, (float)(y + box_height + 20), (float)box_width, (float)box_height };
        button_start = { (float)x, (float)(y + box_height + 20 + 200), (float)box_width, (float)box_height };
        //


        //Settings screen width box text box
        screenWidthBox = { (float)(width / 2 - 190), (float)(global_height / 2 - 80), 180, 50 };
        spinnerWidthEnable = 0;
        //

        //Settings screen height box text box
        screenHeightBox = { (float)(width / 2 + 10), (float)(global_height / 2 - 80), 180, 50 };
        spinnerHeightEnable = 0;
        //

        waveSelect.channels = WAVESELECT_CHANNELS;
        waveSelect.frameCount = WAVESELECT_FRAME_COUNT;
        waveSelect.sampleRate = WAVESELECT_SAMPLE_RATE;
        waveSelect.sampleSize = WAVESELECT_SAMPLE_SIZE;
        waveSelect.data = waveSelectData;

        InitAudioDevice();
        InitWindow(global_width,global_height,"Editor");
        SetTargetFPS(30);
        if(fullscreen && !IsWindowFullscreen())
        {
            ToggleFullscreen();
        }

        //Load Textures
        title = LoadTexture("..//resources//title_main.png");
    }

    void Settings_check()
    {
        if(fp == NULL && !update)
        {
            fclose(fp);
            fp = fopen("..//config//settings.txt", "w+");
            if(fp == NULL)
            {
                fail_open = 1;
            }
            export_width = global_width;
            export_height = global_height;
            export_fullscreen = fullscreen;
            fprintf(fp, "%d %d %d ", export_width, export_height, export_fullscreen);
            fclose(fp);
            fp = fopen("..//config//settings.txt", "r+");
            if(fp == NULL)
            {
                fail_open = 1;
            }
            printf("%d %d %d\n", export_width, export_height, export_fullscreen);
        }
        if(update)
        {
            fclose(fp);
            fp = fopen("..//config//settings.txt", "w+");
            if(fp == NULL)
            {
                fail_open = 1;
            }
            fprintf(fp, "%d %d %d ", export_width, export_height, export_fullscreen);
            fclose(fp);
            printf("%d %d %d\n", export_width, export_height, export_fullscreen);
            update = 0;
            fp = fopen("..//config//settings.txt", "r+");
            if(fp == NULL)
            {
                fail_open = 1;
            }
        }
    }

    void Draw()
    {
        char msg_set[9] = "Settings", msg_ext[6] = "Extra", msg_start[6] = "Start";
        char msg_error[95] = "settings.txt file failed to load and was recreated. \nPlease Press \'Esc\' to quit and relaunch.";
        int textsize;
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(title, global_width /2 - title.width / 2, 80, WHITE); // Draw Settings texture (to change)

        if(fp != NULL && !fail_open)
        {
            //General Menu
            if(current_screen == 0)
            {
                settings_screenWidth = global_width;
                settings_screenHeight = global_height;
                if(GuiButton(button_settings, msg_set))
                {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    current_screen = 1;
                }

                if(GuiButton(button_extra, msg_ext))
                {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    current_screen = 2;
                }

                if(GuiButton(button_start, msg_start))
                {
                    PlaySound(LoadSoundFromWave(waveSelect));
                    current_screen = 3;
                }
            }

            //Settings menu
            if(current_screen == 1)
            {
                //Draw screen width text box
                if(GuiSpinner(screenWidthBox, NULL, &settings_screenWidth, 0, 9999, spinnerWidthEnable))
                {
                    spinnerWidthEnable = !spinnerWidthEnable;
                }
                //

                //Draw screen height text box
                if(GuiSpinner(screenHeightBox, NULL, &settings_screenHeight, 0, 9999, spinnerHeightEnable))
                {
                    spinnerHeightEnable = !spinnerHeightEnable;
                }
                //

                // Draw full-screen switch
                if(GuiButton((Rectangle)
            {
                (float)(global_width / 2 - 90), (float)(global_height - 200), 180, 60
                }, "Fullscreen"))
                {
                    fullscreen = !fullscreen;
                    ToggleFullscreen();
                }
                //

                // Draw save button
                if(GuiButton((Rectangle)
            {
                (float)(global_width / 2 - 90), (float)(global_height - 100), 180, 60
                }, "Save"))
                {
                    global_width = settings_screenWidth;
                    global_height = settings_screenHeight;
                    export_width = global_width;
                    export_height = global_height;
                    export_fullscreen = fullscreen;
                    current_screen = 0;
                    int x = global_width / 2 - 90;
                    int y = global_height * 0.35;
                    int box_width = 180;
                    int box_height = 60;
                    button_settings = { (float)x, (float)y, (float)box_width, (float)box_height };
                    button_extra = { (float)x, (float)(y + box_height + 20), (float)box_width, (float)box_height};
                    button_start = { (float)x, (float)(y + box_height + 20 + 200), (float)box_width, (float)box_height};
                    update = 1;
                    Settings_check();           //Export All
                    SetWindowSize(export_width, export_height);
                    screenWidthBox = { (float)(global_width / 2 - 190), (float)(global_height / 2 - 80), 180, 50 };
                    screenHeightBox = { (float)(global_width / 2 + 10), (float)(global_height / 2 - 80), 180, 50 };
                }
                //
            }

            //Extra menu
            if(current_screen == 2)
            {
                textsize = MeasureText("Nothing but us chickens here!", 30);
                DrawTextEx(font, "Nothing but us chickens here!", (Vector2)
                {
                    (float)(global_width / 2 - textsize / 2), (float)(global_height / 2 - 15)
                }, 30, 1,BLACK);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    current_screen = 0;
                }
            }
        }
        else
        {
            textsize = MeasureText(msg_error,25);
            DrawTextEx(font,msg_error, (Vector2)
            {
                (float)(global_width / 2 - textsize / 2), (float)(global_height / 2)
            }, 25, 1,BLACK);
        }

        EndDrawing();
    }

    void Logic()
    {
        if(IsKeyPressed('F'))
        {
            ToggleFullscreen();
        }

        if(WindowShouldClose())
        {
            quit = 1;
        }
    }

    int Run(int fail)
    {
        fail_open = fail;
        Settings_check();
        while(!quit && current_screen != 3)
        {
            Draw();
            Logic();
            if(feof(fp))
            {
                rewind(fp);
            }
        }
        UnloadTexture(title);
        return quit;
    }
};

typedef struct Bricks
{
    Vector2 position;
    int brickWidth;
    int brickHeight;
    int type;           //0 - Normal, 1 - 1HP, 2 - 2HP, 3 - Gold(Unbreakable), 4 - Explosive
    bool enabled;
    bool selected;
} Bricks;

class cGameManager
{
private:
    FILE *fp;       // pointer for file to save level
    //FILE *fs;       // pointer for file to load settings
    Bricks *brick;                  // max amount of bricks; example: brick[100] [100] is 100 x 100 bricks = 10000
    int brickCount;
    Image imgBrick;
    Image imgSelect;
    Texture2D texBrick;
    Texture2D texSelect;
    bool resizeTexture;

    Wave waveSelect;

    Rectangle button_SaveAndQuit;   // Save and quit button
    Rectangle button_QuitOnly;      // Quit only button
    bool save;                      // To save or not to save...that is the question (I had to I was listening to Hate Me)
    bool quit;
public:
    cGameManager()
    {
        fp = NULL;
        //fs = NULL;
        //fs = fopen("..//config//settings.txt", "r");

        //variables
        brick = (Bricks *)MemAlloc(MAX_BRICKS*sizeof(Bricks)); // MemAlloc() is equivalent to malloc()
        brickCount = 0;

        imgBrick = LoadImage("..//resources//Breakout-Brick.gif");
        imgSelect = LoadImage("..//resources//Breakout-Brick-Selected.gif");
        ImageResize(&imgBrick, 50, 35);
        ImageResize(&imgSelect, 50, 35);
        texBrick = LoadTextureFromImage(imgBrick);
        texSelect = LoadTextureFromImage(imgSelect);
        resizeTexture = 0;

        button_SaveAndQuit = { (float)(GetScreenWidth() / 2 - 160), (float)(GetScreenHeight() - 100), 150, 50};
        button_QuitOnly = { (float)(GetScreenWidth() / 2 + 10), (float)(GetScreenHeight() - 100), 150, 50};

        waveSelect.channels = WAVESELECT_CHANNELS;
        waveSelect.frameCount = WAVESELECT_FRAME_COUNT;
        waveSelect.sampleRate = WAVESELECT_SAMPLE_RATE;
        waveSelect.sampleSize = WAVESELECT_SAMPLE_SIZE;
        waveSelect.data = waveSelectData;

        save = 0;
        quit = 0;

        SetTargetFPS(240);
    }

    void Init()   // load the brick variable with only ones (meaning all bricks are visible and active)
    {
        for(int i = 0; i < MAX_BRICKS; i++)
        {
            brick[i].brickWidth = 50;
            brick[i].brickHeight = 35;
            brick[i].enabled = 0;
            brick[i].type = 0;
            brick[i].selected = 0;
        }
    }

    int Info()   //the info menu
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // The actual message you see on screen from here on:
        DrawText("Level editor for Brick Breaker Raylib Edition",GetScreenWidth() / 2 - 42 * 3, GetScreenHeight() / 7.6,15,WHITE);
        DrawText("Create the model that will appear in the game.",GetScreenWidth() / 2 - 52 * 3, GetScreenHeight() / 2.5,15,WHITE);
        DrawText("Click + ctrl to create a brick. Click it to select it. Right Click to change it's type.",GetScreenWidth() / 2 - 87 * 3, GetScreenHeight() / 2.5 + 35,15,WHITE);
        DrawText("Press \"Save and Quit\" once you are done to quit and save.",GetScreenWidth() / 2 - 59 * 3, GetScreenHeight() / 2.5 + 70,15,WHITE);
        DrawText("Click left mouse button to continue.",GetScreenWidth() / 2 - 36 * 3, GetScreenHeight() - GetScreenHeight() / 7.6,15,WHITE);

        EndDrawing();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int CheckBrickExists(bool enb_offset = 0)   //Check if a brick exists at mouse location and output the bricks ID out of brickCount (only to be used in an if)
    {
        for(int i = 0; i <= brickCount; i++)
        {
            if(GetMouseX() > brick[i].position.x - ( brick[i].brickWidth / 2 ) * enb_offset && GetMouseX() < brick[i].position.x + brick[i].brickWidth * 1.5 * enb_offset + brick[i].brickWidth * !enb_offset)
            {
                if(GetMouseY() > brick[i].position.y - ( brick[i].brickHeight / 2 ) * enb_offset && GetMouseY() < brick[i].position.y + brick[i].brickHeight * 1.5 + brick[i].brickHeight * !enb_offset)
                {
                    printf("ID of Brick Clicked: %i\n", i);
                    return i;
                }
            }
        }
        printf("No brick clicked. Result: -1\n");
        return -1;
    }

    int CheckBrickCollision()
    {
        int brick_id = CheckBrickExists();
        if(brick_id != -1)
        {
            //int selected_brick_wall_R = brick[brick_id].position.x + brick[brick_id].brickWidth;
            //int selected_brick_wall_B = brick[brick_id].position.y + brick[brick_id].brickHeight;
            for(int i = 0; i <= brickCount; i++)
            {

            }
            return 0;
        }
        return -1;
    }

    void Draw()   //draw the actual bricks only if their enabled value is 1
    {
        BeginDrawing();
        ClearBackground(BLACK);
        //Loop to draw bricks
        Color bColor;
        for(int i = 0; i <= brickCount; i++)
        {
            if(brick[i].enabled)
            {
                if(brick[i].type == 0)
                {
                    bColor = SKYBLUE;
                }
                if(brick[i].type == 1)
                {
                    bColor = BLUE;
                }
                if(brick[i].type == 2)
                {
                    bColor = GRAY;
                }
                if(brick[i].type == 3)
                {
                    bColor = GOLD;
                }
                if(brick[i].type == 4)
                {
                    bColor = ORANGE;
                }
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
                if(brick[i].selected)
                {
                    DrawTexture(texSelect, brick[i].position.x, brick[i].position.y, WHITE);
                }
            }
        }
        //

        // Draw border
        DrawRectangle(0,0,10,GetScreenHeight(),BROWN);
        DrawRectangle(0,0,GetScreenWidth(),10,BROWN);
        DrawRectangle(GetScreenWidth() - 10,0, GetScreenWidth(), GetScreenHeight(),BROWN);
        DrawRectangle(0, GetScreenHeight() - 10, GetScreenWidth(), GetScreenHeight(),BROWN);
        //

        // Draw Quit and SaveAndQuit buttons
        if(GuiButton(button_SaveAndQuit, "Save and Quit"))
        {
            PlaySound(LoadSoundFromWave(waveSelect));
            save = 1;
            quit = 1;
        }
        if(GuiButton(button_QuitOnly, "Quit Only"))
        {
            PlaySound(LoadSoundFromWave(waveSelect));
            save = 0;
            quit = 1;
        }
        //

        //Dev Stuff
        DrawLine(0,GetScreenHeight() - 350, GetScreenWidth(), GetScreenHeight() - 350, RED);
        DrawCircle(GetMouseX(), GetMouseY(), 5, RED);
        DrawText(TextFormat("%i", GetMouseX()), GetMouseX() - 20, GetMouseY() + 5, 15, RED);
        DrawText(TextFormat("%i", GetMouseY()), GetMouseX(), GetMouseY() - 15, 15, RED);
        //

        EndDrawing();

    }
    void Logic()   //
    {
        //Spawn bricks with Left-Click and disallow creating another on top of it
        if(brickCount != MAX_BRICKS - 1)
        {
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL))
            {
                if(GetMouseY() < GetScreenHeight() - 350)
                {
                    if(CheckBrickExists(1) == -1)
                    {
                        brick[brickCount].enabled = 1;
                        brick[brickCount].position = (Vector2)
                        {
                            (float)(GetMouseX() - brick[brickCount].brickWidth / 2), (float)(GetMouseY() - brick[brickCount].brickHeight / 2)
                        };
                        brickCount++;
                        printf("Brick amount on Screen: %d\n", brickCount);
                    }
                }
            }
        }
        //

        //Code to allow to select bricks for extra options
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int a = CheckBrickExists();
            if(a != -1)
            {
                brick[a].selected = 1;
                printf("Brick selected: %d\n", a);
                for(int i = 0; i < a; i++)
                {
                    brick[i].selected = 0;
                }
                for(int i = a + 1; i < brickCount; i++)
                {
                    brick[i].selected = 0;
                }
            }
            else
            {
                for(int i = 0; i < brickCount; i++)
                {
                    brick[i].selected = 0;
                    printf("Nothing selected!\n");
                }
            }
        }
        //

        //Move bricks by holding Left-Click
        // TODO (Dark#7#): Repair brick movement
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            bool a = CheckBrickCollision();  //CheckBrickExists might also function as the collision check between bricks
            printf("Collision result: %i\n", a);
            if(a == 0)
            {
                if(brick[a].selected)
                {
                    brick[a].position = (Vector2)
                    {
                        GetMousePosition().x - brick[a].brickWidth / 2, GetMousePosition().y - brick[a].brickHeight / 2
                    };
                }
            }
        }
        //

        //Change brick type
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            int a = CheckBrickExists();
            if(a != -1)
            {
                if(brick[a].selected)
                {
                    if(brick[a].type != 4)      //????
                    {
                        brick[a].type++;
                    }
                    else
                    {
                        brick[a].type = 0;
                    }
                    switch(brick[a].type)
                    {
                    case 0:
                        printf("Normal\n");
                        break;
                    case 1:
                        printf("1HP\n");
                        break;
                    case 2:
                        printf("2HP\n");
                        break;
                    case 3:
                        printf("Gold(Unbreakable)\n");
                        break;
                    case 4:
                        printf("Explosive\n\n");
                        break;
                    }
                }
            }
        }
        //
    }

    int searchString(char *arrayToParseIn, char stringToSearchFor[], char stringToReplaceWith[])     //Allows you to replace part of string
    {
        int i = 0;
        int arraySize = strlen(arrayToParseIn);
        int searchSize = strlen(stringToSearchFor);

        char temp_compare[searchSize + 1];
        char temp_arrayhead[arraySize + 1];
        char temp_arraybottom[arraySize + 1];
        memset(temp_compare, 0, searchSize*sizeof(char));
        memset(temp_arrayhead, 0, arraySize*sizeof(char));
        memset(temp_arraybottom, 0, arraySize*sizeof(char));

        while(i < arraySize)
        {
            if(arrayToParseIn[i] == stringToSearchFor[0])
            {
                strncpy(temp_compare, &arrayToParseIn[i], searchSize);
                temp_compare[searchSize] = '\0';
                if(strcmp(temp_compare, stringToSearchFor) == 0)
                {
                    strncpy(temp_arrayhead, arrayToParseIn, i);
                    temp_arrayhead[i] = '\0';
                    strncpy(temp_arraybottom, &arrayToParseIn[searchSize + i], arraySize - i - searchSize);
                    temp_arraybottom[arraySize - i - searchSize] = '\0';
                    sprintf(arrayToParseIn, "%s%s%s", temp_arrayhead, stringToReplaceWith, temp_arraybottom);
                    return i;
                }
            }
            i++;
        }
        return -1;
    }

    void Output()   //output the current brick layout to the level.txt file
    {
        /*char charParse[STR_SIZE]; //HOW TO OPEN A FILE USING FOPEN AND LOAD THE FILE INTO A VARIABLE. TO USE WITH SEARCHSTRING
        char charParse[1000];
        char charTemp[100];
        FILE *fp;
        fp = fopen("SupplierData-K190.xml", "r");
        while(fgets(charTemp,sizeof(charTemp), fp) != NULL){
        	strcat(charParse, charTemp);
        	strcat(charParse, "\n");
        }
        fclose(fp);*/
        fp = fopen("..//config//level.txt", "w");
        // output brick layout to level.txt
        for(int i = 0; i <= brickCount - 1; i++)
        {
            fprintf(fp, "%d %d %f %f %d %d %d ", GetScreenWidth(), GetScreenHeight(), brick[i].position.x, brick[i].position.y, brick[i].brickWidth, brick[i].brickHeight, brick[i].type);
            printf("\nBrick Position:\n");
            printf("     X: %f\n", brick[i].position.x);
            printf("     Y: %f\n", brick[i].position.y);
            printf("Brick Width: %d\n", brick[i].brickWidth);
            printf("Brick Height: %d\n", brick[i].brickHeight);
            printf("Brick type: ");
            switch(brick[i].type)
            {
            case 0:
                printf("Normal\n");
                break;
            case 1:
                printf("1HP\n");
                break;
            case 2:
                printf("2HP\n");
                break;
            case 3:
                printf("Gold(Unbreakable)\n");
                break;
            case 4:
                printf("Explosive\n\n");
                break;
            }
        }
        //
    }
    int Run(int quit_settings)   //actually runs the previously made functions
    {
        quit = quit_settings;
        Init();

        bool info_bool = false;
        while(!info_bool & !quit)
        {
            info_bool = Info();
        }

        while(!WindowShouldClose() & !quit)
        {
            if(IsWindowFocused())
            {
                Logic();
            }
            Draw();
        }
        if(save)
        {
            Output();
        }

        _fcloseall();
        MemFree(brick); //equivalent to free() but for memstuff
        return quit;
    }
};

int main()
{
    int width, height, fullscreen, quit = 0, fail = 0;
    FILE *fp = NULL;
    fp = fopen("..//config//settings.txt", "r+");
    if(fp != NULL)
    {
        fscanf(fp, "%d %d %d", &width, &height, &fullscreen);
    }
    else
    {
        printf("Settings failed to open!\n");
        width = 640;
        height = 480;
        fullscreen = 0;
        fail = 1;
    }
    fclose(fp);
    printf("Screen width: %d\n", width);
    printf("Screen height: %d\n", height);
    printf("Fullscreen: %d\n\n", fullscreen);

    cSettings c_settings(width,height,fullscreen);
    quit = c_settings.Run(fail);
    cGameManager c_game;
    quit = !c_game.Run(quit);
    CloseWindow();
    return quit;
}
