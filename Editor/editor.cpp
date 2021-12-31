// TODO: Implement Delta Time for cursor blinking !!!
#include <iostream>
#include "raylib.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstring>
using std::fstream;
using std::endl;
using std::string;
using std::cout;
using std::ios;
using std::strcpy;

#define MAX_INPUT_CHARS   9
#define MAX_INPUT_INT     4

fstream lvl_editor("..//config//level.txt", ios::out);
fstream settings("..//config//settings.txt", ios::in | ios::out);

class cSettings
{
private:
    Texture2D title;
    int export_width, export_height, export_brick_width, export_brick_height, export_fullscreen, export_offset;
    string export_brick_color;
    int current_screen;
    Rectangle button_settings;
    Rectangle button_extra;
    Rectangle button_start;
    int quit;

    Rectangle colorBox;   //the box to write color to in settings
    char settings_color[MAX_INPUT_CHARS + 1];
    int letterCount_color;
    bool mouseOnText_color;
    int framesCounter_color;

    Rectangle screenWidthBox;   //the box to write screen width to in settings
    char settings_screenWidth[MAX_INPUT_CHARS + 1];
    int letterCount_screenWidth;
    bool mouseOnText_screenWidth;
    int framesCounter_screenWidth;

    Rectangle screenHeightBox;  //the box to write screen height to in settings
    char settings_screenHeight[MAX_INPUT_CHARS + 1];
    int letterCount_screenHeight;
    bool mouseOnText_screenHeight;
    int framesCounter_screenHeight;

    Rectangle brickWBox;  //the box to write brick width to in settings
    char settings_brickWidth[MAX_INPUT_CHARS + 1];
    int letterCount_brickWidth;
    bool mouseOnText_brickWidth;
    int framesCounter_brickWidth;

    Rectangle brickHBox;  //the box to write brick height to in settings
    char settings_brickHeight[MAX_INPUT_CHARS + 1];
    int letterCount_brickHeight;
    bool mouseOnText_brickHeight;
    int framesCounter_brickHeight;

    Rectangle offsetBox;  //the box to write offset between bricks to in settings
    char settings_offset[MAX_INPUT_CHARS + 1];
    int letterCount_offset;
    bool mouseOnText_offset;
    int framesCounter_offset;

    Rectangle fullscreen; //the box to toggle full-screen with in settings
    char settings_fullscreen[MAX_INPUT_CHARS + 1];
    int letterCount_fullscreen;
    bool mouseOnText_fullscreen;
    int framesCounter_fullscreen;

public:
    cSettings(int width, int height)
    {
        quit = 0;
        current_screen = 0; // 0 - Menu, 1 - Settings, 2 - Extra, 3 - Start

        //Color settings text box
        settings_color[MAX_INPUT_CHARS + 1] = '\0';
        colorBox = { 150, 250, 225, 50 };
        letterCount_color = 0;
        mouseOnText_color = false;
        framesCounter_color = 0;
        //
        //Settings screen width box text box
        settings_screenWidth[MAX_INPUT_INT + 1] = '\0';
        screenWidthBox = { width - 445, 250, 180, 50 };
        letterCount_screenWidth = 0;
        mouseOnText_screenWidth = false;
        framesCounter_screenWidth = 0;
        //
        //Settings screen height box text box
        settings_screenHeight[MAX_INPUT_INT + 1] = '\0';
        screenHeightBox = { width - 225, 250, 180, 50 };
        letterCount_screenHeight = 0;
        mouseOnText_screenHeight = false;
        framesCounter_screenHeight = 0;
        //
        //Settings brick width box text box
        settings_brickWidth[MAX_INPUT_INT + 1] = '\0';
        brickWBox = { width - 445, 450, 180, 50 };
        letterCount_brickWidth = 0;
        mouseOnText_brickWidth = false;
        framesCounter_brickWidth = 0;
        //
        //Settings brick height box text box
        settings_brickHeight[MAX_INPUT_INT + 1] = '\0';
        brickHBox = { width - 225, 450, 180, 50 };
        letterCount_brickHeight = 0;
        mouseOnText_brickHeight = false;
        framesCounter_brickHeight = 0;
        //
        //Settings offset box text box
        settings_offset[MAX_INPUT_INT + 1] = '\0';
        offsetBox = { 150, 450, 225, 50 };
        letterCount_offset = 0;
        mouseOnText_offset = false;
        framesCounter_offset = 0;
        //
        //Settings fullscreen switch

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
                //Draw Color text box
                DrawText("Color",205,200,40,BLACK);
                DrawRectangleRec(colorBox, LIGHTGRAY);
                if(mouseOnText_color)
                {
                    DrawRectangleLines((int)colorBox.x, (int)colorBox.y, (int)colorBox.width, (int)colorBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)colorBox.x, (int)colorBox.y, (int)colorBox.width, (int)colorBox.height, DARKGRAY);
                }
                DrawText(settings_color, (int)colorBox.x + 5, (int)colorBox.y + 15, 30, MAROON);
                if (mouseOnText_color)
                {
                    if (letterCount_color < MAX_INPUT_CHARS)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_color/20)%2) == 0)
                        {
                            DrawText("_", (int)colorBox.x + 8 + MeasureText(settings_color, 30), (int)colorBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
                }
                //
                //Draw screen width text box
                DrawText("Screen",GetScreenWidth() - 315,160,40,BLACK);
                DrawText("Width           Height",GetScreenWidth() - 390,210,30,BLACK);
                DrawText("X",GetScreenWidth() - 250,265,20,BLACK);
                DrawRectangleRec(screenWidthBox, LIGHTGRAY);
                if(mouseOnText_screenWidth)
                {
                    DrawRectangleLines((int)screenWidthBox.x, (int)screenWidthBox.y, (int)screenWidthBox.width, (int)screenWidthBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)screenWidthBox.x, (int)screenWidthBox.y, (int)screenWidthBox.width, (int)screenWidthBox.height, DARKGRAY);
                }
                DrawText(settings_screenWidth, (int)screenWidthBox.x + 5, (int)screenWidthBox.y + 15, 30, MAROON);
                if (mouseOnText_screenWidth)
                {
                    if (letterCount_screenWidth < MAX_INPUT_INT)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_screenWidth/20)%2) == 0)
                        {
                            DrawText("_", (int)screenWidthBox.x + 8 + MeasureText(settings_screenWidth, 30), (int)screenWidthBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //
                //Draw screen height text box
                DrawRectangleRec(screenHeightBox, LIGHTGRAY);
                if(mouseOnText_screenHeight)
                {
                    DrawRectangleLines((int)screenHeightBox.x, (int)screenHeightBox.y, (int)screenHeightBox.width, (int)screenHeightBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)screenHeightBox.x, (int)screenHeightBox.y, (int)screenHeightBox.width, (int)screenHeightBox.height, DARKGRAY);
                }
                DrawText(settings_screenHeight, (int)screenHeightBox.x + 5, (int)screenHeightBox.y + 15, 30, MAROON);
                if (mouseOnText_screenHeight)
                {
                    if (letterCount_screenHeight < MAX_INPUT_INT)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_screenHeight/20)%2) == 0)
                        {
                            DrawText("_", (int)screenHeightBox.x + 8 + MeasureText(settings_screenHeight, 30), (int)screenHeightBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //
                //Draw brick width text box
                DrawText("Brick",GetScreenWidth() - 295,360,40,BLACK);
                DrawText("Width           Height",GetScreenWidth() - 390,410,30,BLACK);
                DrawText("X",GetScreenWidth() - 250,465,20,BLACK);
                DrawRectangleRec(brickWBox, LIGHTGRAY);
                if(mouseOnText_brickWidth)
                {
                    DrawRectangleLines((int)brickWBox.x, (int)brickWBox.y, (int)brickWBox.width, (int)brickWBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)brickWBox.x, (int)brickWBox.y, (int)brickWBox.width, (int)brickWBox.height, DARKGRAY);
                }
                DrawText(settings_brickWidth, (int)brickWBox.x + 5, (int)brickWBox.y + 15, 30, MAROON);
                if (mouseOnText_brickWidth)
                {
                    if (letterCount_brickWidth < MAX_INPUT_INT)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_brickWidth/20)%2) == 0)
                        {
                            DrawText("_", (int)brickWBox.x + 8 + MeasureText(settings_brickWidth, 30), (int)brickWBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //
                //Draw brick height text box
                DrawRectangleRec(brickHBox, LIGHTGRAY);
                if(mouseOnText_brickHeight)
                {
                    DrawRectangleLines((int)brickHBox.x, (int)brickHBox.y, (int)brickHBox.width, (int)brickHBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)brickHBox.x, (int)brickHBox.y, (int)brickHBox.width, (int)brickHBox.height, DARKGRAY);
                }
                DrawText(settings_brickHeight, (int)brickHBox.x + 5, (int)brickHBox.y + 15, 30, MAROON);
                if (mouseOnText_brickHeight)
                {
                    if (letterCount_brickHeight < MAX_INPUT_INT)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_brickHeight/20)%2) == 0)
                        {
                            DrawText("_", (int)brickHBox.x + 8 + MeasureText(settings_brickHeight, 30), (int)brickHBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //
                //Draw Offset text box
                DrawText("Offset",205,395,40,BLACK);
                DrawRectangleRec(offsetBox, LIGHTGRAY);
                if(mouseOnText_offset)
                {
                    DrawRectangleLines((int)offsetBox.x, (int)offsetBox.y, (int)offsetBox.width, (int)offsetBox.height, RED);
                }
                else
                {
                    DrawRectangleLines((int)offsetBox.x, (int)offsetBox.y, (int)offsetBox.width, (int)offsetBox.height, DARKGRAY);
                }
                DrawText(settings_offset, (int)offsetBox.x + 5, (int)offsetBox.y + 15, 30, MAROON);
                if (mouseOnText_offset)
                {
                    if (letterCount_offset < MAX_INPUT_INT)
                    {
                        // Draw blinking underscore char
                        if (((framesCounter_offset/20)%2) == 0)
                        {
                            DrawText("_", (int)offsetBox.x + 8 + MeasureText(settings_offset, 30), (int)offsetBox.y + 15, 30, MAROON);
                        }
                    }
                    else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
                }
                //
                // Draw full-screen switch
                DrawRectangle(GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60, WHITE);
                DrawRectangleLines(GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60, BLACK);
                int textsize = MeasureText("Fullscreen",30);
                DrawText("Fullscreen", GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 185, 30, BLACK); //Resolution Text
                //
                // Draw save button
                DrawRectangle(GetScreenWidth() / 2 - 90, GetScreenHeight() - 100, 180, 60, WHITE);
                DrawRectangleLines(GetScreenWidth() / 2 - 90, GetScreenHeight() - 100, 180, 60, BLACK);
                textsize = MeasureText("Save",30);
                DrawText("Save", GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 85, 30, BLACK); //Resolution Text
                //
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
            //Logic for input color
            {
                if (CheckCollisionPointRec(GetMousePosition(), colorBox))
                {
                    mouseOnText_color = true;
                }
                else
                {
                    mouseOnText_color = false;
                }
                if(mouseOnText_color)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0) //Only letters allowed
                    {
                        if((key >= 97) && (key <= 122) && (letterCount_color < MAX_INPUT_CHARS))
                        {
                            key = key - 32;
                        }
                        if((key >= 65) && (key <= 90) && (letterCount_color < MAX_INPUT_CHARS))
                        {
                            settings_color[letterCount_color] = (char) key;
                            settings_color[letterCount_color + 1] = '\0';
                            letterCount_color++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_color--;
                        if(letterCount_color < 0)
                        {
                            letterCount_color = 0;
                        }
                        settings_color[letterCount_color] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_color)
                {
                    framesCounter_color++;
                }
                else
                {
                    framesCounter_color = 0;
                }
            }
            //
            // Logic for input screen width
            {
                if (CheckCollisionPointRec(GetMousePosition(), screenWidthBox))
                {
                    mouseOnText_screenWidth = true;
                }
                else
                {
                    mouseOnText_screenWidth = false;
                }
                if(mouseOnText_screenWidth)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0)  //Only numbers allowed (to change)
                    {
                        if((key >= 48) && (key <= 57) && (letterCount_screenWidth < MAX_INPUT_INT))
                        {
                            settings_screenWidth[letterCount_screenWidth] = (char) key;
                            settings_screenWidth[letterCount_screenWidth + 1] = '\0';
                            letterCount_screenWidth++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_screenWidth--;
                        if(letterCount_screenWidth < 0)
                        {
                            letterCount_screenWidth = 0;
                        }
                        settings_screenWidth[letterCount_screenWidth] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_screenWidth)
                {
                    framesCounter_screenWidth++;
                }
                else
                {
                    framesCounter_screenWidth = 0;
                }
            }
            //
            // Logic for input screen height
            {
                if (CheckCollisionPointRec(GetMousePosition(), screenHeightBox))
                {
                    mouseOnText_screenHeight = true;
                }
                else
                {
                    mouseOnText_screenHeight = false;
                }
                if(mouseOnText_screenHeight)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0)  //Only numbers allowed (to change)
                    {
                        if((key >= 48) && (key <= 57) && (letterCount_screenHeight < MAX_INPUT_INT))
                        {
                            settings_screenHeight[letterCount_screenHeight] = (char) key;
                            settings_screenHeight[letterCount_screenHeight + 1] = '\0';
                            letterCount_screenHeight++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_screenHeight--;
                        if(letterCount_screenHeight < 0)
                        {
                            letterCount_screenHeight = 0;
                        }
                        settings_screenHeight[letterCount_screenHeight] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_screenHeight)
                {
                    framesCounter_screenHeight++;
                }
                else
                {
                    framesCounter_screenHeight = 0;
                }
            }
            //
            // Logic for input brick width
            {
                if (CheckCollisionPointRec(GetMousePosition(), brickWBox))
                {
                    mouseOnText_brickWidth = true;
                }
                else
                {
                    mouseOnText_brickWidth = false;
                }
                if(mouseOnText_brickWidth)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0)  //Only numbers allowed (to change)
                    {
                        if((key >= 48) && (key <= 57) && (letterCount_brickWidth < MAX_INPUT_INT))
                        {
                            settings_brickWidth[letterCount_brickWidth] = (char) key;
                            settings_brickWidth[letterCount_brickWidth + 1] = '\0';
                            letterCount_brickWidth++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_brickWidth--;
                        if(letterCount_brickWidth < 0)
                        {
                            letterCount_brickWidth = 0;
                        }
                        settings_brickWidth[letterCount_brickWidth] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_brickWidth)
                {
                    framesCounter_brickWidth++;
                }
                else
                {
                    framesCounter_brickWidth = 0;
                }
            }
            //
            // Logic for input brick height
            {
                if (CheckCollisionPointRec(GetMousePosition(), brickHBox))
                {
                    mouseOnText_brickHeight = true;
                }
                else
                {
                    mouseOnText_brickHeight = false;
                }
                if(mouseOnText_brickHeight)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0)  //Only numbers allowed (to change)
                    {
                        if((key >= 48) && (key <= 57) && (letterCount_brickHeight < MAX_INPUT_INT))
                        {
                            settings_brickHeight[letterCount_brickHeight] = (char) key;
                            settings_brickHeight[letterCount_brickHeight + 1] = '\0';
                            letterCount_brickHeight++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_brickHeight--;
                        if(letterCount_brickHeight < 0)
                        {
                            letterCount_brickHeight = 0;
                        }
                        settings_brickHeight[letterCount_brickHeight] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_brickHeight)
                {
                    framesCounter_brickHeight++;
                }
                else
                {
                    framesCounter_brickHeight = 0;
                }
            }
            //
            // Logic for input offset
            {
                if (CheckCollisionPointRec(GetMousePosition(), offsetBox))
                {
                    mouseOnText_offset = true;
                }
                else
                {
                    mouseOnText_offset = false;
                }
                if(mouseOnText_offset)
                {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0)  //Only numbers allowed (to change)
                    {
                        if((key >= 48) && (key <= 57) && (letterCount_offset < MAX_INPUT_INT))
                        {
                            settings_offset[letterCount_offset] = (char) key;
                            settings_offset[letterCount_offset + 1] = '\0';
                            letterCount_offset++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount_offset--;
                        if(letterCount_offset < 0)
                        {
                            letterCount_offset = 0;
                        }
                        settings_offset[letterCount_offset] = '\0';
                    }
                }
                else
                {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_offset)
                {
                    framesCounter_offset++;
                }
                else
                {
                    framesCounter_offset = 0;
                }
            }
            //
            // Logic for input fullscreen
            if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60}))
            {
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ToggleFullscreen();
                }
            }
            //
            // Logic for save changes button and actions to do
            if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth() / 2 - 90, GetScreenHeight() - 100, 180, 60}))
            {
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    if(letterCount_color == 0)
                    {
                        cout<<"Empty"<<endl;
                        settings_color[0] = 'R';
                        settings_color[1] = 'E';
                        settings_color[2] = 'D';
                        cout<<settings_color<<endl;
                    }
                    else
                    {
                        cout<<settings_color<<endl;
                    }
                    if(letterCount_screenWidth == 0)
                    {
                        export_width = 1024;
                        strcpy(settings_screenWidth, "1024");
                        letterCount_screenWidth = 4;
                        cout<<export_width<<endl;
                    }
                    else
                    {
                        cout<<settings_screenWidth<<endl;
                        export_width = atoi(settings_screenWidth);
                        cout<<export_width<<endl;
                    }
                    if(letterCount_screenHeight == 0)
                    {
                        export_height = 760;
                        strcpy(settings_screenHeight, "760");
                        letterCount_screenHeight = 3;
                        cout<<export_height<<endl;
                    }
                    else
                    {
                        cout<<settings_screenHeight<<endl;
                        export_height = atoi(settings_screenHeight);
                        cout<<export_height<<endl;
                    }
                    if(letterCount_brickWidth == 0)
                    {
                        export_brick_width = 50;
                        strcpy(settings_brickWidth, "50");
                        letterCount_brickWidth = 2;
                        cout<<export_brick_width<<endl;
                    }
                    else
                    {
                        cout<<settings_brickWidth<<endl;
                        export_brick_width = atoi(settings_brickWidth);
                        cout<<export_brick_width<<endl;
                    }
                    if(letterCount_brickHeight == 0)
                    {
                        export_brick_height = 35;
                        strcpy(settings_brickHeight, "35");
                        letterCount_brickHeight = 2;
                        cout<<export_brick_height<<endl;
                    }
                    else
                    {
                        cout<<settings_brickHeight<<endl;
                        export_brick_height = atoi(settings_brickHeight);
                        cout<<export_brick_height<<endl;
                    }
                    if(letterCount_offset == 0)
                    {
                        export_offset = 10;
                        strcpy(settings_offset, "10");
                        letterCount_offset = 2;
                        cout<<export_offset<<endl;
                    }
                    else
                    {
                        cout<<settings_offset<<endl;
                        export_offset = atoi(settings_offset);
                        cout<<export_offset<<endl;
                    }
                    current_screen = 0;
                }
            }
            //
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
