#include "cSettings.h"

// TODO (Administrator#9#02/10/24): Complete main menu and remove useless leftovers

bool spinnerWidthEnable = 0;
bool spinnerHeightEnable = 0;
bool isError = 0;
extern bool quit;

bool settingsOutput() { //1 - FAIL, 0 - SUCCESS
    FILE *fp;
    char path[255];
    sprintf(path, "%s\\config\\settings.txt", GetPrevDirectoryPath(GetWorkingDirectory()));
    fp = fopen(path, "w"); //Open settings.txt file
    if(fp == nullptr) {
        fclose(fp);
        fp = fopen(path, "w+");
        if(fp == NULL) {
            return 1;
        }
    }
    int exportWidth = GetScreenWidth(), exportHeight = GetScreenHeight(),  isFullscreen = IsWindowFullscreen();
    fprintf(fp, "%d %d %d", exportWidth, exportHeight, isFullscreen);
#ifdef _DEBUG
    printf("%d %d %d\n", exportWidth, exportHeight, isFullscreen);
#endif
    fclose(fp);
    return 0;
}

void drawSettings(int &currentScreen, int &newScreenWidth,  int &newScreenHeight) {
    if(!isError) {
        //Settings screen width box text box
        Rectangle screenWidthBox = { (float)(GetScreenWidth() / 2 - 190), (float)(GetScreenHeight() / 2 - 80), 180, 50 };
        //Settings screen height box text box
        Rectangle screenHeightBox = { (float)(GetScreenWidth() / 2 + 10), (float)(GetScreenHeight() / 2 - 80), 180, 50 };

        //Draw screen width text box
        if(GuiSpinner(screenWidthBox, NULL, &newScreenWidth, 0, 9999, spinnerWidthEnable)) {
            spinnerWidthEnable = !spinnerWidthEnable;
        }
        //Draw screen height text box
        if(GuiSpinner(screenHeightBox, NULL, &newScreenHeight, 0, 9999, spinnerHeightEnable)) {
            spinnerHeightEnable = !spinnerHeightEnable;
        }
        // Draw full-screen switch
        if(GuiButton((Rectangle) {
        (float)(GetScreenWidth() / 2 - 90), (float)(GetScreenHeight() - 200), 180, 60
        }, "Fullscreen")) {
            ToggleFullscreen();
        }
        // Draw save button
        if(GuiButton((Rectangle) {
        (float)(GetScreenWidth() / 2 - 90), (float)(GetScreenHeight() - 100), 180, 60
        }, "Save")) {
            SetWindowSize(newScreenWidth, newScreenHeight);
            bool isFailed = settingsOutput();           //Export All
            if(isFailed)
                isError = 1;
            else
                currentScreen = 0;
        }
    } else {
        char msg_error[255] = "Settings file could not be updated.\nPress button to quit.\n";
        int textsize = MeasureText(msg_error,25);
        DrawText(msg_error, (float)(GetScreenWidth() / 2 - textsize / 2), (float)(GetScreenHeight() / 2), 25, BLACK);
        if(GuiButton((Rectangle) {
        (float)(GetScreenWidth() / 2 - 90), (float)(GetScreenHeight() - 200), 180, 60
        }, "Quit")) {
            quit = 1;
        }
    }
}
