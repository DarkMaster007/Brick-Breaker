//  TODO:
//  Allow the player to load the existing level
//  Allow the player to not replace existing level file
//  Allow to resize the bricks at will
//  Consolidate if's into either switch's of less if's

#include <iostream>
#include "raylib.h"
#include <fstream>
#include <cstring>
using std::fstream;
using std::endl;
using std::string;
using std::cout;
using std::ios;
using std::strcpy;

#define MAX_INPUT_CHARS   9
#define MAX_INPUT_INT     4
#define MAX_BRICKS        300

fstream settings;
fstream lvl_editor;
fstream game_settings;

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
    bool update;
    int fail_open;

    Rectangle screenWidthBox;   //the box to write screen width to in settings
    char settings_screenWidth[MAX_INPUT_INT + 1];
    int letterCount_screenWidth;
    bool mouseOnText_screenWidth;
    int framesCounter_screenWidth;

    Rectangle screenHeightBox;  //the box to write screen height to in settings
    char settings_screenHeight[MAX_INPUT_INT + 1];
    int letterCount_screenHeight;
    bool mouseOnText_screenHeight;
    int framesCounter_screenHeight;

    Rectangle fullscreen; //the box to toggle full-screen with in settings
    char settings_fullscreen[MAX_INPUT_INT + 1];
    int letterCount_fullscreen;
    bool mouseOnText_fullscreen;
    int framesCounter_fullscreen;

public:
    cSettings(int width, int height)
    {
        quit = 0;
        current_screen = 0; // 0 - Menu, 1 - Settings, 2 - Extra, 3 - Start
        update = 0;
        fail_open = 0;
        export_fullscreen = 0;
        if(!settings.is_open()) {
            settings.open("..//config//settings.txt", ios::out | ios::in);
        }

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

        InitWindow(width,height,"Editor");
        SetTargetFPS(30);
        //Load Textures
        title = LoadTexture("..//resources//title_main.png");
    }
    void Settings_check()
    {
        if(!settings.is_open() & !update) {
            settings.close();
            settings.open("..//config//settings.txt", ios::out | ios::in | ios::trunc);
            export_width = 1024;
            export_height = 768;
            export_fullscreen = 0;
            settings<<export_width<<" "<<export_height<<" "<<export_fullscreen<<" ";
            settings.close();
            settings.open("..//config//settings.txt", ios::out | ios::in);
            cout<<export_width<<" "<<export_height<<" "<<export_fullscreen<<endl;
        }
        if(update) {
            settings.close();
            settings.open("..//config//settings.txt", ios::out | ios::in | ios::trunc);
            settings<<export_width<<" "<<export_height<<" "<<export_fullscreen;
            settings.close();
            cout<<export_width<<" "<<export_height<<" "<<export_fullscreen<<endl;
            update = 0;
            settings.open("..//config//settings.txt", ios::out | ios::in);
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

        if(settings.is_open() & !fail_open) {
            //General Menu
            if(current_screen == 0) {
                button_settings = { GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.65 * GetScreenHeight(), 180, 60 };
                DrawRectangleRec(button_settings, WHITE);   //Settings Button
                textsize = MeasureText(msg_set,30);
                DrawText(msg_set, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.65 * GetScreenHeight() + 15, 30, BLACK);

                button_extra = {GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.55 * GetScreenHeight(), 180, 60};
                DrawRectangleRec(button_extra, WHITE); //Extra Button
                textsize = MeasureText(msg_ext,30);
                DrawText(msg_ext, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.55 * GetScreenHeight() + 20, 30, BLACK);

                button_start = {GetScreenWidth() / 2 - 90, GetScreenHeight() - 0.35 * GetScreenHeight(), 180, 60};
                DrawRectangleRec(button_start, WHITE); //Start Button
                textsize = MeasureText(msg_start,30);
                DrawText(msg_start, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() - 0.35 * GetScreenHeight() + 15, 30, BLACK); //Start Text
            }

            //Settings menu
            if(current_screen == 1) {
                //Draw screen width text box
                DrawText("Screen",GetScreenWidth() - 315,160,40,BLACK);
                DrawText("Width           Height",GetScreenWidth() - 390,210,30,BLACK);
                DrawText("X",GetScreenWidth() - 250,265,20,BLACK);
                DrawRectangleRec(screenWidthBox, LIGHTGRAY);
                if(mouseOnText_screenWidth) {
                    DrawRectangleLines((int)screenWidthBox.x, (int)screenWidthBox.y, (int)screenWidthBox.width, (int)screenWidthBox.height, RED);
                } else {
                    DrawRectangleLines((int)screenWidthBox.x, (int)screenWidthBox.y, (int)screenWidthBox.width, (int)screenWidthBox.height, DARKGRAY);
                }
                DrawText(settings_screenWidth, (int)screenWidthBox.x + 5, (int)screenWidthBox.y + 15, 30, MAROON);
                if (mouseOnText_screenWidth) {
                    if (letterCount_screenWidth < MAX_INPUT_INT) {
                        // Draw blinking underscore char
                        if (((framesCounter_screenWidth/20)%2) == 0) {
                            DrawText("_", (int)screenWidthBox.x + 8 + MeasureText(settings_screenWidth, 30), (int)screenWidthBox.y + 15, 30, MAROON);
                        }
                    } else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //

                //Draw screen height text box
                DrawRectangleRec(screenHeightBox, LIGHTGRAY);
                if(mouseOnText_screenHeight) {
                    DrawRectangleLines((int)screenHeightBox.x, (int)screenHeightBox.y, (int)screenHeightBox.width, (int)screenHeightBox.height, RED);
                } else {
                    DrawRectangleLines((int)screenHeightBox.x, (int)screenHeightBox.y, (int)screenHeightBox.width, (int)screenHeightBox.height, DARKGRAY);
                }
                DrawText(settings_screenHeight, (int)screenHeightBox.x + 5, (int)screenHeightBox.y + 15, 30, MAROON);
                if (mouseOnText_screenHeight) {
                    if (letterCount_screenHeight < MAX_INPUT_INT) {
                        // Draw blinking underscore char
                        if (((framesCounter_screenHeight/20)%2) == 0) {
                            DrawText("_", (int)screenHeightBox.x + 8 + MeasureText(settings_screenHeight, 30), (int)screenHeightBox.y + 15, 30, MAROON);
                        }
                    } else DrawText("Press BACKSPACE to delete chars...", 150, 300, 20, GRAY);
                }
                //

                // Draw full-screen switch
                DrawRectangle(GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60, WHITE);
                DrawRectangleLines(GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60, BLACK);
                textsize = MeasureText("Fullscreen",30);
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
            if(current_screen == 2) {
                textsize = MeasureText("Nothing but us chickens here!",25);
                DrawText("Nothing but us chickens here!", GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() / 2, 25, BLACK);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    current_screen = 0;
                }
            }
        } else {
            textsize = MeasureText(msg_error,25);
            DrawText(msg_error, GetScreenWidth() / 2 - textsize / 2, GetScreenHeight() / 2, 25, BLACK);
        }

        EndDrawing();
    }
    void Logic()
    {
        //Collision for the main menu buttons: settings, extra and start
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if(CheckCollisionPointRec(GetMousePosition(),button_settings)) {
                current_screen = 1;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_extra)) {
                current_screen = 2;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_start)) {
                current_screen = 3;
            }
        }

        if(WindowShouldClose()) {
            quit = 1;
        }

        //Collision for the settings input boxes and buttons
        if(current_screen == 1) {
            // Logic for input screen width
            {
                if (CheckCollisionPointRec(GetMousePosition(), screenWidthBox)) {
                    mouseOnText_screenWidth = true;
                } else {
                    mouseOnText_screenWidth = false;
                }
                if(mouseOnText_screenWidth) {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0) { //Only numbers allowed (to change)
                        if((key >= 48) && (key <= 57) && (letterCount_screenWidth < MAX_INPUT_INT)) {
                            settings_screenWidth[letterCount_screenWidth] = (char) key;
                            settings_screenWidth[letterCount_screenWidth + 1] = '\0';
                            letterCount_screenWidth++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE)) {
                        letterCount_screenWidth--;
                        if(letterCount_screenWidth < 0) {
                            letterCount_screenWidth = 0;
                        }
                        settings_screenWidth[letterCount_screenWidth] = '\0';
                    }
                } else {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_screenWidth) {
                    framesCounter_screenWidth++;
                } else {
                    framesCounter_screenWidth = 0;
                }
            }
            //

            // Logic for input screen height
            {
                if (CheckCollisionPointRec(GetMousePosition(), screenHeightBox)) {
                    mouseOnText_screenHeight = true;
                } else {
                    mouseOnText_screenHeight = false;
                }
                if(mouseOnText_screenHeight) {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    int key = GetCharPressed();
                    while(key>0) { //Only numbers allowed (to change)
                        if((key >= 48) && (key <= 57) && (letterCount_screenHeight < MAX_INPUT_INT)) {
                            settings_screenHeight[letterCount_screenHeight] = (char) key;
                            settings_screenHeight[letterCount_screenHeight + 1] = '\0';
                            letterCount_screenHeight++;
                        }
                        key = GetCharPressed();
                    }
                    if(IsKeyPressed(KEY_BACKSPACE)) {
                        letterCount_screenHeight--;
                        if(letterCount_screenHeight < 0) {
                            letterCount_screenHeight = 0;
                        }
                        settings_screenHeight[letterCount_screenHeight] = '\0';
                    }
                } else {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
                if(mouseOnText_screenHeight) {
                    framesCounter_screenHeight++;
                } else {
                    framesCounter_screenHeight = 0;
                }
            }
            //

            // Logic for input full-screen
            if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth() / 2 - 90, GetScreenHeight() - 200, 180, 60})) {
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    export_fullscreen = !export_fullscreen;
                    ToggleFullscreen();

                }
            }
            //

            // Logic for save changes button and actions to do
            if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth() / 2 - 90, GetScreenHeight() - 100, 180, 60})) {
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    //Set Export Screen Width
                    if(letterCount_screenWidth == 0) {
                        export_width = 1024;
                        strcpy(settings_screenWidth, "1024");
                        letterCount_screenWidth = 4;
                        cout<<export_width<<endl;
                    } else {
                        cout<<settings_screenWidth<<endl;
                        export_width = atoi(settings_screenWidth);
                        cout<<export_width<<endl;
                    }
                    //

                    //Set Export Screen Height
                    if(letterCount_screenHeight == 0) {
                        export_height = 760;
                        strcpy(settings_screenHeight, "760");
                        letterCount_screenHeight = 3;
                        cout<<export_height<<endl;
                    } else {
                        cout<<settings_screenHeight<<endl;
                        export_height = atoi(settings_screenHeight);
                        cout<<export_height<<endl;
                    }
                    //Set Export full-screen state
                    cout<<export_fullscreen<<endl;

                    current_screen = 0;
                    update = 1;
                    Settings_check();           //Export All
                    SetWindowSize(export_width, export_height);
                }
            }
            //
        }
    }
    int Run(int fail)
    {
        fail_open = fail;
        Settings_check();
        while(!quit & current_screen != 3) {
            Draw();
            Logic();
            if(settings.eof()) {
                settings.seekg(ios::beg);
                settings.clear();
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
    Bricks *brick;                  // max amount of bricks; example: brick[100] [100] is 100 x 100 bricks = 10000
    int brickCount;

    Image imgBrick;
    Image imgSelect;
    Texture2D texBrick;
    Texture2D texSelect;
    bool resizeTexture;

    bool fullscreen;                // whether it's full-screen or not
    Rectangle button_SaveAndQuit;   // Save and quit button
    Rectangle button_QuitOnly;      // Quit only button
    bool save;                      // To save or not to save...that is the question (I had to I was listening to Hate Me)
    bool quit;
public:
    cGameManager()
    {
        game_settings.open("..//config//settings.txt", ios::in);

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

        save = 0;
        quit = 0;

        SetTargetFPS(240);
    }

    void Init() // load the brick variable with only ones (meaning all bricks are visible and active)
    {
        for(int i = 0; i < MAX_BRICKS; i++) {
            brick[i].brickWidth = 50;
            brick[i].brickHeight = 35;
            brick[i].enabled = 0;
            brick[i].type = 0;
            brick[i].selected = 0;
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
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return true;
        } else {
            return false;
        }
    }

    int CheckBrickExists()  //Check if a brick exists at mouse location and output the bricks ID out of brickCount (only to be used in an if)
    {
        for(int i = 0; i <= brickCount; i++) {
            if(GetMouseX() > brick[i].position.x - brick[i].brickWidth / 2 && GetMouseX() < brick[i].position.x + brick[i].brickWidth * 1.5) {
                if(GetMouseY() > brick[i].position.y - brick[i].brickHeight / 2 && GetMouseY() < brick[i].position.y + brick[i].brickHeight * 1.5) {
                    //cout<<"ID of Brick Clicked: "<<i<<endl;
                    return i;
                }
            }
        }
        //cout<<"No brick clicked. Result: -1"<<endl;
        return -1;
    }

    void Draw()  //draw the actual bricks only if their enabled value is 1
    {
        BeginDrawing();
        ClearBackground(BLACK);
        //Loop to draw bricks
        Color bColor;
        for(int i = 0; i <= brickCount; i++) {
            if(brick[i].enabled) {
                if(brick[i].type == 0) {
                    bColor = SKYBLUE;
                }
                if(brick[i].type == 1) {
                    bColor = BLUE;
                }
                if(brick[i].type == 2) {
                    bColor = GRAY;
                }
                if(brick[i].type == 3) {
                    bColor = GOLD;
                }
                if(brick[i].type == 4) {
                    bColor = ORANGE;
                }
                DrawTexture(texBrick, brick[i].position.x, brick[i].position.y, bColor);
                if(brick[i].selected) {
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
        button_SaveAndQuit = {GetScreenWidth() / 2 - 160, GetScreenHeight() - 100, 150, 50};
        button_QuitOnly = {GetScreenWidth() / 2 + 10, GetScreenHeight() - 100, 150, 50};
        DrawRectangleRec(button_SaveAndQuit,WHITE);
        DrawRectangleRec(button_QuitOnly,WHITE);
        int textsize = MeasureText("Only Quit",25);
        DrawText("Only Quit", GetScreenWidth() / 2 + 30, GetScreenHeight() - 85, 25, BLACK);
        textsize = MeasureText("Quit and save",20);
        DrawText("Quit and save", GetScreenWidth() / 2 - 155, GetScreenHeight() - 85, 20, BLACK);
        //

        //Dev Stuff
        DrawLine(0,GetScreenHeight() - 350, GetScreenWidth(), GetScreenHeight() - 350, RED);
        DrawCircle(GetMouseX(), GetMouseY(), 5, RED);
        DrawText(TextFormat("%i", GetMouseX()), GetMouseX() - 20, GetMouseY() + 5, 15, RED);
        DrawText(TextFormat("%i", GetMouseY()), GetMouseX(), GetMouseY() - 15, 15, RED);
        //

        EndDrawing();

    }
    void Logic()  //
    {
        //Spawn bricks with Left-Click and disallow creating another on top of it
        if(brickCount != MAX_BRICKS - 1) {
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) {
                if(GetMouseY() < GetScreenHeight() - 350) {
                    if(CheckBrickExists() == -1) {
                        brick[brickCount].enabled = 1;
                        brick[brickCount].position = (Vector2) {
                            GetMouseX() - brick[brickCount].brickWidth / 2, GetMouseY() - brick[brickCount].brickHeight / 2
                        };
                        brickCount++;
                        cout<<"Brick amount on Screen: "<<brickCount<<endl;
                    }
                }
            }
        }
        //

        //Code to allow to select bricks for extra options
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            int a = CheckBrickExists();
            if(a != -1) {
                brick[a].selected = 1;
                cout<<"Brick selected: "<<a<<endl;
                for(int i = 0; i < a; i++) {
                    brick[i].selected = 0;
                }
                for(int i = a + 1; i < brickCount; i++) {
                    brick[i].selected = 0;
                }
            } else {
                for(int i = 0; i < brickCount; i++) {
                    brick[i].selected = 0;
                    cout<<"Nothing selected!"<<endl;
                }
            }
        }
        //

        //Move bricks by holding Left-Click
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int a = CheckBrickExists();
            if(a != -1) {
                if(brick[a].selected) {
                    brick[a].position = (Vector2) {
                        GetMousePosition().x - brick[a].brickWidth / 2, GetMousePosition().y - brick[a].brickHeight / 2
                    };
                }
            }
        }
        //

        //Change brick type
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            int a = CheckBrickExists();
            if(a != -1) {
                if(brick[a].selected) {
                    if(brick[a].type != 4) {
                        brick[a].type++;
                    } else {
                        brick[a].type = 0;
                    }
                    if(brick[a].type == 0) {
                        cout<<"Normal"<<endl;
                    }
                    if(brick[a].type == 1) {
                        cout<<"1HP"<<endl;
                    }
                    if(brick[a].type == 2) {
                        cout<<"2HP"<<endl;
                    }
                    if(brick[a].type == 3) {
                        cout<<"Gold(Unbreakable)"<<endl;
                    }
                    if(brick[a].type == 4) {
                        cout<<"Explosive"<<endl;
                    }
                }
            }
        }
        //

        //The Collision for the "Quit Only" and "Save and Quit" buttons
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if(CheckCollisionPointRec(GetMousePosition(),button_SaveAndQuit)) {     //Save and quit
                save = 1;
                quit = 1;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_QuitOnly)) {        //Quit only
                save = 0;
                quit = 1;
            }
        }
        //
    }
    void Output()  //output the current brick layout to the level.txt file
    {
        lvl_editor.open("..//config//level.txt", ios::out);
        // output brick layout to level.txt
        for(int i = 0; i <= brickCount - 1; i++) {
            lvl_editor<<brick[i].position.x<<" "<<brick[i].position.y<<" "<<brick[i].brickWidth<<" "<<brick[i].brickHeight<<" "<<brick[i].type<<" ";
            cout<<endl;
            cout<<"Brick Position:"<<endl;
            cout<<"     X: "<<brick[i].position.x<<endl;
            cout<<"     Y: "<<brick[i].position.y<<endl;
            cout<<"Brick Width: "<<brick[i].brickWidth<<endl;
            cout<<"Brick Height: "<<brick[i].brickHeight<<endl;
            cout<<"Brick type: ";
            if(brick[i].type == 0) {
                cout<<"Normal"<<endl;
            }
            if(brick[i].type == 1) {
                cout<<"1HP"<<endl;
            }
            if(brick[i].type == 2) {
                cout<<"2HP"<<endl;
            }
            if(brick[i].type == 3) {
                cout<<"Gold(Unbreakable)"<<endl;
            }
            if(brick[i].type == 4) {
                cout<<"Explosive"<<endl;
            }
            cout<<endl;
        }
        //
    }
    int Run(int quit_settings)  //actually runs the previously made functions
    {
        quit = quit_settings;
        Init();

        bool info_bool = false;
        while(!info_bool & !quit) {
            info_bool = Info();
        }

        while(!WindowShouldClose() & !quit) {
            if(IsWindowFocused()) {
                Logic();
            }
            Draw();
        }
        if(save) {
            Output();
        }

        // close all streams
        if(lvl_editor.is_open()) {
            lvl_editor.close();
        }
        if(game_settings.is_open()) {
            game_settings.close();
        }
        if(settings.is_open()) {
            settings.close();
        }
        MemFree(brick); //equivalent to free()
        return quit;
    }
};

int main(int argc, char** argv)
{
    int width, height, quit = 0, run = 1, fail = 0;
    settings.open("..//config//settings.txt", ios::out | ios::in);
    if(settings.is_open()) {
        settings>>width>>height;
    } else {
        cout<<"Settings failed to open!"<<endl;
        width = 640;
        height = 480;
        fail = 1;
    }
    cout<<"Screen width: "<<width<<endl;
    cout<<"Screen height: "<<height<<endl;
    cSettings c_settings(width,height);
    quit = c_settings.Run(fail);
    cGameManager c_game;
    quit = !c_game.Run(quit);
    CloseWindow();
    return quit;
}
