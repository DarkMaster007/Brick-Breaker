#include <iostream>
#include "raylib.h"
#include <fstream>
#include <string>
using std::fstream;
using std::endl;
using std::string;
using std::cout;
using std::ios;

#define MAX_INPUT_CHARS   9
#define MAX_INPUT_INT     4

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
fstream lvl_editor("..//config//level.txt", ios::out);
fstream settings("..//config//settings.txt", ios::in | ios::out);

class cSettings
{
private:
    Texture2D title;
    int export_width, export_height, export_brick_width, export_brick_height, export_fullscreen;
    string export_brick_color;
    int current_screen;
    Rectangle button_settings;
    Rectangle button_extra;
    Rectangle button_start;
    int quit;
    char settings_color[MAX_INPUT_CHARS + 1];
    int letterCount;
    Rectangle colorBox;
    bool mouseOnText;
    int framesCounter;
public:
    cSettings(int width, int height)
    {
        quit = 0;
        current_screen = 0; // 0 - Menu, 1 - Settings, 2 - Extra, 3 - Start

        //Delete or move these variable, same for private declaration. Only for testing here. SERIOUS!!!
        settings_color[MAX_INPUT_CHARS + 1] = '         \0';
        letterCount = 0;
        colorBox = { width/2.0f - 100, 180, 225, 50 };
        mouseOnText = false;
        framesCounter = 0;
        //
        InitWindow(width,height,"Editor");
        SetTargetFPS(60);
        //Load Textures
        title = LoadTexture("..//resources//title_main.png");
    }
    void Settings_check()
    {
        if(!settings.is_open())
        {
            settings.close();
            fstream settings("..//config//settings.txt", ios::in | ios::out | ios::app);
            export_width = 1024;
            export_height = 768;
            export_brick_width = 50;
            export_brick_height = 35;
            export_fullscreen = 0;
            export_brick_color = "RED";
            settings<<export_width<<" "<<export_height<<" "<<export_brick_width<<" "<<export_brick_height<<" "<<export_fullscreen<<" "<<export_brick_color;
            cout<<export_width<<" "<<export_height<<" "<<export_brick_width<<" "<<export_brick_height<<" "<<export_fullscreen<<" "<<export_brick_color;
        }
    }
    void Draw()
    {
        char msg_set[9] = "Settings", msg_ext[6] = "Extra", msg_start[6] = "Start";
        char msg_error[95] = "settings.txt file failed to load and was recreated. \nPlease Press \'Esc\' to quit and relaunch.";
        int textsize;
        ClearBackground(SKYBLUE);
        BeginDrawing();

        DrawTexture(title, GetScreenWidth() /2 - title.width / 2, 80, WHITE); // Draw Settings texture (to change)
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if(CheckCollisionPointRec(GetMousePosition(),Rectangle {GetScreenWidth() / 2 - title.width / 2, 80, title.width, title.height}))
            {
                cout<<"TITLE!"<<endl;
            }
        }
        if(!settings.fail())
        {
            //General Menu
            if(current_screen == 0)
            {
                button_settings = { GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.65 * GetScreenHeight(), 180, 60 };
                DrawRectangleRec(button_settings, WHITE);   //Settings Button
                textsize = MeasureText(msg_set,30);
                DrawText(msg_set, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.65 * GetScreenHeight() + 15, 30, BLACK); //Resolution Text

                button_extra = {GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.55 * GetScreenHeight(), 180, 60};
                DrawRectangleRec(button_extra, WHITE); //Extra Button
                textsize = MeasureText(msg_ext,30);
                DrawText(msg_ext, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.55 * GetScreenHeight() + 20, 30, BLACK); //Brick and paddle Text

                button_start = {GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.35 * GetScreenHeight(), 180, 60};
                DrawRectangleRec(button_start, WHITE); //Start Button
                textsize = MeasureText(msg_start,30);
                DrawText(msg_start, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.35 * GetScreenHeight() + 15, 30, BLACK); //Start Text
            }

            //Settings menu
            if(current_screen == 1)
            {
                DrawRectangleRec(colorBox, LIGHTGRAY);
                if(mouseOnText)
                {
                    DrawRectangleLines((int)colorBox.x, (int)colorBox.y, (int)colorBox.width, (int)colorBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)colorBox.x, (int)colorBox.y, (int)colorBox.width, (int)colorBox.height, DARKGRAY);
                }
                DrawText(settings_color, (int)colorBox.x + 5, (int)colorBox.y + 15, 30, MAROON);
                if (mouseOnText)
                {
                    if (letterCount < MAX_INPUT_CHARS)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter/20)%2) == 0)
                        {
                            DrawText("_", (int)colorBox.x + 8 + MeasureText(settings_color, 30), (int)colorBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
                }
            }

            //Extra menu
            if(current_screen == 2)
            {

            }
        }
        else
        {
            textsize = MeasureText(msg_error,25);
            DrawText(msg_error, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() / 2, 25, BLACK); //Start Text
        }

        EndDrawing();
    }
    void Logic()
    {
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if(CheckCollisionPointRec(GetMousePosition(),button_settings))
            {
                current_screen = 1;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_extra))
            {
                current_screen = 2;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_start))
            {
                current_screen = 3;
            }

        }
        if(WindowShouldClose())
        {
            quit = 1;
        }
        if(current_screen == 1)
        {
            //Logic for input width
            if (CheckCollisionPointRec(GetMousePosition(), colorBox))
            {
                mouseOnText = true;
            }
            else
            {
                mouseOnText = false;
            }
            if(mouseOnText)
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                int key = GetCharPressed();
                while(key>0)
                {
                    if((key >= 97) && (key <= 122) && (letterCount < MAX_INPUT_CHARS)){
                        key = key - 32;
                    }
                    if((key >= 65) && (key <= 90) && (letterCount < MAX_INPUT_CHARS))
                    {
                        settings_color[letterCount] = (char) key;
                        settings_color[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    letterCount--;
                    if(letterCount < 0)
                    {
                        letterCount = 0;
                    }
                    settings_color[letterCount] = '\0';
                }
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
            if(mouseOnText)
            {
                framesCounter++;
            }
            else
            {
                framesCounter = 0;
            }
            // Logic for input height

            // Logic for save changes button and actions to do
        }
    }
    void Input()
    {
        //No you
    }
    int Run()
    {
        Settings_check();
        while(!quit & current_screen != 3)
        {
            Draw();
            Logic();
            Input();
        }
        UnloadTexture(title);
        return quit;
    }
};

class cGameManager
{
private:
    int offset;                     // distance between bricks / 2 (don't ask); loaded from settings.txt
    int brick [100][100];           // max amount of bricks; example: brick[100] [100] is 100 x 100 bricks = 10000
    int brick_width, brick_height;  // width and height of individual bricks; loaded from settings.txt
    int brick_columns, brick_rows;  // how many rows and how many columns of bricks there should be; calculated
    string brick_color;               // the bricks color; loaded from settings.txt
    bool fullscreen;                // whether it's full-screen or not
    int brick_centering;            // amount of pixels to offset bricks to center them (looks nice :) ); calculated
public:
    cGameManager(int width = 640, int height = 480)
    {
        //variables
        offset = 10;
        settings>>brick_width>>brick_height;
        settings>>fullscreen;
        settings>>brick_color;

        //calculations
        brick_columns = GetScreenWidth() / (brick_width + offset + 4);  // amount of bricks length
        brick_rows = (GetScreenHeight() / 2) / (brick_height + offset); // amount of bricks height
        brick_centering = (GetScreenWidth() - brick_columns * brick_width - brick_columns * offset + offset) / 4;
        //brick_centering = 0;

        //cout's for testing; dev shit, remove from final
        cout<<"Brick width: "<<brick_width<<endl;
        cout<<"Brick height: "<<brick_height<<endl;
        cout<<"Full-screen: "<<fullscreen<<endl;
        cout<<"Color value: "<<brick_color<<endl;
        cout<<"Value to center bricks with: "<<brick_centering<<endl;
        cout<<"Brick columns: "<<brick_columns<<endl<<"Brick rows: "<<brick_rows<<endl;
        cout<<"Bricks total: "<<brick_columns*brick_rows<<endl;

        //InitWindow(w,h,"Editor");
        //SetTargetFPS(60);
    }
    void Init() // load the brick variable with only ones (meaning all bricks are visible and active)
    {
        for(int i = 0; i < brick_columns; i++)    // columns
        {
            for(int j = 0; j < brick_rows; j++)    // rows
            {
                brick[i][j] = 1;
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
    }
    int Info()  //the info menu
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // The actual message you see on screen from here on:
        DrawText("Level editor for Brick Breaker BGM Edition",GetScreenWidth() / 2 - 42 * 3, GetScreenHeight() / 7.6,15,WHITE);
        DrawText("Create the model that will appear in the final game.",GetScreenWidth() / 2 - 52 * 3, GetScreenHeight() / 2.5,15,WHITE);
        DrawText("Click on any brick to disable it and click again to re-enable it.",GetScreenWidth() / 2 - 65 * 3, GetScreenHeight() / 2.5 + 35,15,WHITE);
        DrawText("Press Q once you are done to quit and save.",GetScreenWidth() / 2 - 43 * 3, GetScreenHeight() / 2.5 + 70,15,WHITE);
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
    void Draw()  //draw the actual bricks only if their associated value is 1
    {
        ClearBackground(BLACK);
        BeginDrawing();
        for(int i = 0; i < brick_columns; i++)
        {
            for(int j = 0; j < brick_rows; j++)
            {
                if(brick[i][j])
                {
                    DrawRectangle(brick_centering + (offset * 2) + i * brick_width + i * offset, offset * 2 + j * brick_height + j * offset, brick_width, brick_height,colors());
                }
            }
            // Draw border
            DrawRectangle(0,0,offset,GetScreenHeight(),BROWN);
            DrawRectangle(0,0,GetScreenWidth(),offset,BROWN);
            DrawRectangle(GetScreenWidth() - offset,0, GetScreenWidth(), GetScreenHeight(),BROWN);
            DrawRectangle(0, GetScreenHeight() - offset, GetScreenWidth(), GetScreenHeight(),BROWN);
        }
        EndDrawing();
    }
    void Logic()  //gets mouse location and sets bricks value to 0 if the click happened within the bricks confines
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            for(int i = 0; i < brick_columns; i++)
            {
                for(int j = 0; j < brick_rows; j++)
                {
                    if((GetMouseX() >= (brick_centering + offset * 2 + i * brick_width + i * offset)) & (GetMouseX() <= (brick_centering + offset * 2 + i * brick_width + i * offset + brick_width)) & (GetMouseY() >= (offset * 2 + j * brick_height + j * offset)) & (GetMouseY() <= (offset * 2 + j * brick_height + j * offset + brick_height)))
                    {
                        brick[i][j] = !brick[i][j];
                    }
                }
            }
        }
        if(IsKeyPressed('D'))
        {
            brick_centering++;
            cout<<"Brick centering value: "<<brick_centering<<endl;
        }
        if(IsKeyPressed('A'))
        {
            brick_centering--;
            cout<<"Brick centering value: "<<brick_centering<<endl;
        }
    }
    void Output()  //output the current brick layout to the level.txt file
    {
        // output brick placement to level.txt
        for(int j = 0; j < brick_rows; j++)
        {
            for(int i = 0; i < brick_columns; i++)
            {
                lvl_editor<<brick[i][j]<<" ";
            }
            lvl_editor<<endl;
        }

        // close all streams
        lvl_editor.close();
        settings.close();
    }
    void Run(int quit)  //actually runs the previously made functions
    {
        Init();
        //CheckPattern();
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
        //CheckPattern();
        Output();
    }
};

int main(int argc, char** argv)
{
    int width, height, quit = 0, run = 1;
    if(settings.is_open())
    {
        settings>>width>>height;
    }
    else
    {
        cout<<"Settings failed to open!"<<endl;
        width = 640;
        height = 480;
    }
    cout<<"Screen width: "<<width<<endl;
    cout<<"Screen height: "<<height<<endl;
    while(run)
    {
        cSettings c_settings(width,height);
        quit = c_settings.Run();
        cGameManager c_game(width,height);
        c_game.Run(quit);
        CloseWindow();
        return 0;
    }
}
