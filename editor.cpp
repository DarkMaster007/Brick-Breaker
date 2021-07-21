//#include <iostream>
#include <graphics.h>
#include <fstream>
#include <string.h>
using std::ofstream;
using std::ifstream;
using std::endl;
using std::string;
//using std::cout;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
ofstream lvl_editor("..//config//level.txt");
ifstream settings("..//config//settings.txt");
class cGameManager {
	private:
		int w, h;
		int offset;
		int brick [20][20];
		int bar_w, bar_h;
		int brick_w, brick_h;
		int bar_x, bar_y;
		bool quit;
		int mouse_x, mouse_y;
		char wnd[15];
	public:
		cGameManager(int width = 640, int height = 480, string wnd_name = "Editor") {
			w = width;
			h = height;
			strcpy(wnd,wnd_name.c_str());
			offset = 10;
			settings>>bar_w>>bar_h;
			brick_w = (w - (bar_w + offset)) / (bar_w + offset);
			brick_h = (h / 2) / (bar_h + offset);
			bar_x = (bar_w + offset);
			bar_y = (bar_h + offset);
			quit = false;
			mouse_x, mouse_y;
			initwindow(w,h,wnd);
			setvisualpage(0);
			setactivepage(1);
		}
		void Info() {
			for(int i = 0; i < brick_w; i++) {  // columns
				for(int j = 0; j < brick_h; j++) {  // rows
					brick[i][j] = 1;
				}
			}
			/*for(int j = 0; j < brick_h; j++) {
				for(int i = 0; i < brick_w; i++) {
					cout<<brick[i][j]<<" ";
				}
				cout<<endl;
			}*/
			
			// The actual message you see on screen from here on:
		settextjustify(CENTER_TEXT,CENTER_TEXT);
			outtextxy(w / 2, h / 7.6,"Level editor for Brick Breaker BGM Edition");
			outtextxy(w / 2, h / 2.5,"Create the model that will appear in the final game.");
			outtextxy(w / 2, h / 2.5 + 35,"Click on any brick to disable it and click again to reenable it.");
			outtextxy(w / 2, h / 2.5 + 70,"Press Q once you are done to quit and save.");
			outtextxy(w / 2, h - h / 7.6, "Press any key to continue.");
			swapbuffers();
			getch();
		}
		void Draw() {
			cleardevice();
			for(int i = 0; i < brick_w; i++) {
				for(int j = 0; j < brick_h; j++) {
					if(brick[i][j]) {
						bar(i * bar_x + (offset * 2), j * bar_y + (offset * 2), i * bar_x +(offset * 2) + bar_w, j * bar_y +(offset * 2) + bar_h);
					}
				}
				// Edge draw
				bar(0,0,offset,h);
				bar(0,0,w,offset);
				bar(w - offset,0, w, h);
				bar(0, h - offset, w, h);
			}

			swapbuffers();
		}
		void Input() {
			if(GetAsyncKeyState('Q')) {
				quit = 1;
			}

		}
		void Logic() {
			if(ismouseclick(WM_LBUTTONUP)) {
				mouse_x = 0;
				mouse_y = 0;
				getmouseclick(WM_LBUTTONUP, mouse_x, mouse_y);

				for(int i = 0; i < brick_w; i++) {
					for(int j = 0; j < brick_h; j++) {
						if((mouse_x >= i * bar_x + (offset * 2)) & (mouse_x <= i * bar_x + (offset * 2) + bar_w) & (mouse_y >= j * bar_y + (offset * 2)) & (mouse_y <= j * bar_y + (offset * 2) + bar_h)) {
							brick[i][j] = !brick[i][j];
						}
					}
				}
			}
		}
		void Output() {
			/*for(int j = 0; j < brick_h; j++) {
			for(int i = 0; i < brick_w; i++) {
			cout<<brick[i][j]<<" ";
			}
			cout<<endl;
			}*/

			// output brick placement
			for(int j = 0; j < brick_h; j++) {
				for(int i = 0; i < brick_w; i++) {
					lvl_editor<<brick[i][j]<<" ";
				}
				lvl_editor<<endl;
			}

			// close all streams
			lvl_editor.close();
			settings.close();
		}
		void Run() {
			Info();
			while(!quit) {
				if(GetForegroundWindow() == FindWindowA(NULL,wnd)) {
				Input();
				Logic();}
				Draw();
				delay(50);
			}
			Output();
		}
};

int main(int argc, char** argv) {
	int w, h;
	settings>>w>>h;
	cGameManager c(w,h);
	c.Run();
	return 0;
}