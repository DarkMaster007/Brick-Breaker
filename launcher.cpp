#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <string.h>
#include <fstream>
using namespace std;

ifstream settings("config//settings.txt");
//ofstream settings_o("..//config//settings.txt");
class cLauncher {
	private:
		int w, h;
		int state;
		int mouse_x, mouse_y;
		int quit;
		char wnd[15];
		int next;
	public:
		cLauncher(int width = 650, int height = 480, string wnd_name = "Launcher") {
			w = width;
			h = height;
			strcpy(wnd, wnd_name.c_str());
			state = 1;
			quit = 0;
			next = 0;
			initwindow(w,h,wnd);
			setvisualpage(0);
			setactivepage(1);
		}
		void mouse_reset() {
			mouse_x = 0;
			mouse_y = 0;
		}
		void Draw() {
			cleardevice();
			if(state == 1) {  // Main menu
				// Make values more relative
				readimagefile("resources//button_brickbreaker.bmp", w / 2 - 70, h / 2 - 50, w / 2 + 70, h / 2);
				readimagefile("resources//button_editor.bmp", w / 2 - 70, h / 2 + 20, w / 2 + 70, h / 2 + 70);
				readimagefile("resources//button_exit.bmp", w / 2 - 70, h / 2 + 90, w / 2 + 70, h / 2 + 140);
				readimagefile("resources//title_main.gif",w / 2 - 200,70, w / 2 + 200,140);
				readimagefile("resources//button_settings.bmp",20,20,160,70);
			}
			if(state == 2) {  // Brick menu
				// Make values more relative
				readimagefile("resources//game.bmp", w / 2, h / 2 - h / 4, w / 2 + 300, h / 2 + h / 4);
				rectangle(20,h / 2 - 120, w / 2 - 20, h / 2 + 200);
				readimagefile("resources//text_brickbreaker.gif", 25, h / 2 - 115, w / 2 - 25, h / 2 + 195);
				readimagefile("resources//button_start.bmp", w / 2 + 80, (3 * h) / 4 + 10, w / 2 + 220, (3 * h) / 4 + 60);
				readimagefile("resources//button_back.bmp",20,20,160,70);
			}
			if(state == 3) {  // Editor menu
				// Make values more relative
				readimagefile("resources//editor.bmp", w / 2, h / 2 - h / 4, w / 2 + 300, h / 2 + h / 4);
				rectangle(20,h / 2 - 120, w / 2 - 20, h / 2 + 200);
				readimagefile("resources//text_editor.gif", 25, h / 2 - 115, w / 2 - 25, h / 2 + 195);
				readimagefile("resources//button_start.bmp",w / 2 + 80,(3 * h) / 4 + 10, w / 2 + 220, (3 * h) / 4 + 60);
				readimagefile("resources//button_back.bmp",20,20,160,70);

			}
			if(state == 4) { // Settings menu
				rectangle(w / 2 - 70, h / 5, w / 2 + 70, h / 5 + 50);
			}
			swapbuffers();
		}
		void Input() {
			if(GetAsyncKeyState('Q')) {
				quit = 1;
			}
			if(ismouseclick(WM_LBUTTONUP)) {
				getmouseclick(WM_LBUTTONUP, mouse_x, mouse_y);
			}

		}
		void Logic() {
			// Main menu buttons
			if(state == 1 & (mouse_x > w / 2 - 70 & mouse_x < w / 2 + 70) & (mouse_y > h / 2 - 50 & mouse_y < h / 2)) { // Brick breaker button
				state = 2;
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
			}
			if(state == 1 & (mouse_x > w / 2 - 70 & mouse_x < w / 2 + 70) & (mouse_y > h / 2 + 20 & mouse_y < h / 2 + 70)) { // Editor button
				state = 3;
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
			}
			if(state == 1 & (mouse_x > 20 & mouse_x < 160) & (mouse_y > 20 & mouse_y < 70)) {
				state = 4;
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
			}
			if((state == 1 & mouse_x > w / 2 - 70 & mouse_x < w / 2 + 70) & (mouse_y > h / 2 + 90 & mouse_y < h / 2 + 140)) { // Editor button
				quit = 1;
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
			}

			//Brick Breaker screen button
			if(state == 2 & (mouse_x > w / 2 + 80 & mouse_x < w / 2 + 220) & (mouse_y > (3 * h) / 4 + 10 & mouse_y < h / 2 + 195)) { // Start button
				ShellExecute( NULL, "open", "Game.exe", NULL, "bin", SW_SHOWNORMAL );
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
				next = 1;
			}
			// Editor screen button
			if(state == 3 & (mouse_x > w / 2 + 80 & mouse_x < w / 2 + 220) & (mouse_y > (3 * h) / 4 + 10 & mouse_y < h / 2 + 195)) {
				ShellExecute( NULL, "open", "Editor.exe", NULL, "bin", SW_SHOWNORMAL );
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
				next = 1;
			}
			// Back button to send back to main menu
			if(state != 1 & (mouse_x > 20 & mouse_x < 160) & (mouse_y > 20 & mouse_y < 85)) {
				state = 1;
				mouse_reset();
				PlaySoundA((LPCSTR) "resources//sounds//Blip_Select_P.wav",NULL,SND_FILENAME | SND_ASYNC );
			}
			/*cout << GetForegroundWindow() << endl;
			cout << FindWindowA(NULL,"Launcher") << endl;*/
		}
		void Run() {
			while(!(quit | next)) {
				if(GetForegroundWindow() == FindWindowA(NULL,wnd)) {
					Input();
					Logic();
				}
				Draw();
			}
		}
};

int main(int argc, char** argv) {
	int temp;
	int w, h;
	settings>>w>>h;
	cout<<w<<" "<<h;
	cLauncher c(w,h);
	c.Run();
	//ShellExecute( NULL, "open", "Brick Breaker.exe", NULL, NULL, SW_SHOWNORMAL );
	return 0;
}