//  TODO:
//  Allow the player to load the existing level
//  Allow the player to not replace existing level file
//  Allow to resize the bricks at will
//  Consolidate if's into either switch's of less if's
#define RAYGUI_IMPLEMENTATION

#include <iostream>
#include "raylib.h"
#include "raygui.h"
#include <fstream>
#include <cstring>
#include <math.h>
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
    Font font;
    Texture2D title;
    int export_width, export_height, export_brick_width, export_brick_height, export_fullscreen, export_offset;
    string export_brick_color;
    int current_screen;
    int global_width, global_height;
    int fullscreen;
    Rectangle button_settings;
    Rectangle button_extra;
    Rectangle button_start;
    int quit;
    bool update;
    int fail_open;

    Rectangle screenWidthBox;   //the box to write screen width to in settings
    int settings_screenWidth;
    bool spinnerWidthEnable;

    Rectangle screenHeightBox;  //the box to write screen height to in settings
    int settings_screenHeight;
    bool spinnerHeightEnable;

public:
    cSettings(int width, int height, int fullscreen_loaded)
    {
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
        if(!settings.is_open())
        {
            settings.open("..//config//settings.txt", ios::out | ios::in);
        }

        //Main screen
        int x = global_width / 2 - 90;
        int y = global_height * 0.35;
        int box_width = 180;
        int box_height = 60;
        button_settings = { x, y, box_width, box_height };
        button_extra = {x, y + box_height + 20, box_width, box_height};
        button_start = {x, y + box_height + 20 + 200, box_width, box_height};
        //


        //Settings screen width box text box
        screenWidthBox = { width / 2 - 190, global_height / 2 - 80, 180, 50 };
        spinnerWidthEnable = 0;
        //

        //Settings screen height box text box
        screenHeightBox = { width / 2 + 10, global_height / 2 - 80, 180, 50 };
        spinnerHeightEnable = 0;
        //

        InitWindow(global_width,global_height,"Editor");
        SetTargetFPS(30);
        if(fullscreen && !IsWindowFullscreen()){
            ToggleFullscreen();
        }

        //Load Textures
        title = LoadTexture("..//resources//title_main.png");
    }

    void Settings_check()
    {
        if(!settings.is_open() && !update)
        {
            settings.close();
            settings.open("..//config//settings.txt", ios::out | ios::in | ios::trunc);
            export_width = global_width;
            export_height = global_height;
            export_fullscreen = fullscreen;
            settings<<export_width<<" "<<export_height<<" "<<export_fullscreen<<" ";
            settings.close();
            settings.open("..//config//settings.txt", ios::out | ios::in);
            cout<<export_width<<" "<<export_height<<" "<<export_fullscreen<<endl;
        }
        if(update)
        {
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
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(title, global_width /2 - title.width / 2, 80, WHITE); // Draw Settings texture (to change)

        if(settings.is_open() & !fail_open)
        {
            //General Menu
            if(current_screen == 0)
            {
                settings_screenWidth = global_width;
                settings_screenHeight = global_height;
                if(GuiButton(button_settings, msg_set)) {current_screen = 1;}

                if(GuiButton(button_extra, msg_ext)) {current_screen = 2;}

                if(GuiButton(button_start, msg_start)) {current_screen = 3;}
            }

            //Settings menu
            if(current_screen == 1)
            {
                //Draw screen width text box
                if(GuiSpinner(screenWidthBox, NULL, &settings_screenWidth, 0, 9999, spinnerWidthEnable)) {spinnerWidthEnable = !spinnerWidthEnable;}
                //

                //Draw screen height text box
                if(GuiSpinner(screenHeightBox, NULL, &settings_screenHeight, 0, 9999, spinnerHeightEnable)) {spinnerHeightEnable = !spinnerHeightEnable;}
                //

                // Draw full-screen switch
                if(GuiButton((Rectangle){global_width / 2 - 90, global_height - 200, 180, 60}, "Fullscreen")) {
                    fullscreen = !fullscreen;
                    ToggleFullscreen();
                }
                //

                // Draw save button
                if(GuiButton((Rectangle){global_width / 2 - 90, global_height - 100, 180, 60}, "Save")) {
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
                    button_settings = { x, y, box_width, box_height };
                    button_extra = {x, y + box_height + 20, box_width, box_height};
                    button_start = {x, y + box_height + 20 + 200, box_width, box_height};
                    update = 1;
                    Settings_check();           //Export All
                    SetWindowSize(export_width, export_height);
                    screenWidthBox = { global_width / 2 - 190, global_height / 2 - 80, 180, 50 };
                    screenHeightBox = { global_width / 2 + 10, global_height / 2 - 80, 180, 50 };
                }
                //
            }

            //Extra menu
            if(current_screen == 2)
            {
                textsize = MeasureText("Nothing but us chickens here!", 30);
                DrawTextEx(font, "Nothing but us chickens here!", (Vector2){global_width / 2 - textsize / 2, global_height / 2 - 15}, 30, 1,BLACK);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    current_screen = 0;
                }
            }
        }
        else
        {
            textsize = MeasureText(msg_error,25);
            DrawTextEx(font ,msg_error, (Vector2){global_width / 2 - textsize / 2, global_height / 2}, 25, 1,BLACK);
        }

        EndDrawing();
    }

    void Logic()
    {
        if(IsKeyPressed('F')){
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
        while(!quit & current_screen != 3)
        {
            Draw();
            Logic();
            if(settings.eof())
            {
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
        for(int i = 0; i < MAX_BRICKS; i++)
        {
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
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int CheckBrickExists()  //Check if a brick exists at mouse location and output the bricks ID out of brickCount (only to be used in an if)
    {
        for(int i = 0; i <= brickCount; i++)
        {
            if(GetMouseX() > brick[i].position.x - brick[i].brickWidth / 2 && GetMouseX() < brick[i].position.x + brick[i].brickWidth * 1.5)
            {
                if(GetMouseY() > brick[i].position.y - brick[i].brickHeight / 2 && GetMouseY() < brick[i].position.y + brick[i].brickHeight * 1.5)
                {
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
        if(brickCount != MAX_BRICKS - 1)
        {
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL))
            {
                if(GetMouseY() < GetScreenHeight() - 350)
                {
                    if(CheckBrickExists() == -1)
                    {
                        brick[brickCount].enabled = 1;
                        brick[brickCount].position = (Vector2)
                        {
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
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            int a = CheckBrickExists();
            if(a != -1)
            {
                brick[a].selected = 1;
                cout<<"Brick selected: "<<a<<endl;
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
                    cout<<"Nothing selected!"<<endl;
                }
            }
        }
        //

        //Move bricks by holding Left-Click
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            int a = CheckBrickExists();  //CheckBrickExists might also function as the collision check between bricks
            if(a != -1)
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
                    if(brick[a].type != 4)
                    {
                        brick[a].type++;
                    }
                    else
                    {
                        brick[a].type = 0;
                    }
                    if(brick[a].type == 0)
                    {
                        cout<<"Normal"<<endl;
                    }
                    if(brick[a].type == 1)
                    {
                        cout<<"1HP"<<endl;
                    }
                    if(brick[a].type == 2)
                    {
                        cout<<"2HP"<<endl;
                    }
                    if(brick[a].type == 3)
                    {
                        cout<<"Gold(Unbreakable)"<<endl;
                    }
                    if(brick[a].type == 4)
                    {
                        cout<<"Explosive"<<endl;
                    }
                }
            }
        }
        //

        //The Collision for the "Quit Only" and "Save and Quit" buttons
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if(CheckCollisionPointRec(GetMousePosition(),button_SaveAndQuit))       //Save and quit
            {
                save = 1;
                quit = 1;
            }
            if(CheckCollisionPointRec(GetMousePosition(),button_QuitOnly))          //Quit only
            {
                save = 0;
                quit = 1;
            }
        }
        //
    }
    
    int searchString(char *arrayToParseIn, char stringToSearchFor[], char stringToReplaceWith[]) {   //Allows you to replace part of string
	int i = 0;
	int arraySize = strlen(arrayToParseIn);
	int searchSize = strlen(stringToSearchFor);

	char temp_compare[searchSize + 1];
	char temp_arrayhead[arraySize + 1];
	char temp_arraybottom[arraySize + 1];
	memset(temp_compare, 0, searchSize*sizeof(char));
	memset(temp_arrayhead, 0, arraySize*sizeof(char));
	memset(temp_arraybottom, 0, arraySize*sizeof(char));

	while(i < arraySize) {
		if(arrayToParseIn[i] == stringToSearchFor[0]) {
			strncpy(temp_compare, &arrayToParseIn[i], searchSize);
			temp_compare[searchSize] = '\0';
			if(strcmp(temp_compare, stringToSearchFor) == 0) {
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
    
    void Output()  //output the current brick layout to the level.txt file
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
        lvl_editor.open("..//config//level.txt", ios::out);
        // output brick layout to level.txt
        for(int i = 0; i <= brickCount - 1; i++)
        {
            lvl_editor<<GetScreenWidth()<<" "<<GetScreenHeight()<<" "<<brick[i].position.x<<" "<<brick[i].position.y<<" "<<brick[i].brickWidth<<" "<<brick[i].brickHeight<<" "<<brick[i].type<<" ";
            cout<<endl;
            cout<<"Brick Position:"<<endl;
            cout<<"     X: "<<brick[i].position.x<<endl;
            cout<<"     Y: "<<brick[i].position.y<<endl;
            cout<<"Brick Width: "<<brick[i].brickWidth<<endl;
            cout<<"Brick Height: "<<brick[i].brickHeight<<endl;
            cout<<"Brick type: ";
            if(brick[i].type == 0)
            {
                cout<<"Normal"<<endl;
            }
            if(brick[i].type == 1)
            {
                cout<<"1HP"<<endl;
            }
            if(brick[i].type == 2)
            {
                cout<<"2HP"<<endl;
            }
            if(brick[i].type == 3)
            {
                cout<<"Gold(Unbreakable)"<<endl;
            }
            if(brick[i].type == 4)
            {
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

        // close all streams
        if(lvl_editor.is_open())
        {
            lvl_editor.close();
        }
        if(game_settings.is_open())
        {
            game_settings.close();
        }
        if(settings.is_open())
        {
            settings.close();
        }
        MemFree(brick); //equivalent to free()
        return quit;
    }
};

int main(int argc, char** argv)
{
    int width, height, fullscreen, quit = 0, run = 1, fail = 0;
    settings.open("..//config//settings.txt", ios::out | ios::in);
    if(settings.is_open())
    {
        settings>>width>>height>>fullscreen;
    }
    else
    {
        cout<<"Settings failed to open!"<<endl;
        width = 640;
        height = 480;
        fullscreen = 0;
        fail = 1;
    }
    cout<<"Screen width: "<<width<<endl;
    cout<<"Screen height: "<<height<<endl;
    cout<<"Fullscreen: "<<fullscreen<<endl;

    cSettings c_settings(width,height,fullscreen);
    quit = c_settings.Run(fail);
    cGameManager c_game;
    quit = !c_game.Run(quit);
    CloseWindow();
    return quit;
}
