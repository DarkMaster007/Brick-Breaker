/*
	Name:
	Copyright:
	Author:
	Date: 05/07/21 20:47

	To add:
	* Pause Menu
	* Win Menu
	* random number generator for the ball movement to make it slightly random
*/
#include <iostream>
#include <fstream>
#include <graphics.h>
#include <string.h>
using namespace std;

ifstream lvl_builder("..//config//level.txt");
ifstream settings("..//config//settings.txt");
enum eDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4};

class cBall {
	private:
		int x, y;
		int originalX, originalY;
		eDir direction;
	public:
		cBall(int posX, int posY) {
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
			direction = STOP;
		}
		void Reset() {
			x = originalX;
			y = originalY;
			direction = STOP;
		}
		void changeDirection(eDir d) {
			direction = d;
		}
		void randomDirection() {
			direction = (eDir)((rand() % 4) + 1);
		}
		inline int getX() {
			return x;
		}
		inline int getY() {
			return y;
		}
		inline eDir getDirection() {
			return direction;
		}
		void Move() {
			switch (direction) {
				case STOP:
					break;
				case UPLEFT:
					x-=3;
					y-=3;
					break;
				case DOWNLEFT:
					x-=3;
					y+=3;
					break;
				case UPRIGHT:
					x+=3;
					y-=3;
					break;
				case DOWNRIGHT:
					x+=3;
					y+=3;
					break;
				default:
					break;
			}
		}
};
class cPaddle {
	private:
		int x, y;
		int originalX, originalY;
	public:
		cPaddle(int posX, int posY) {
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
		}
		inline int getX() {
			return x;
		}
		inline int getY() {
			return y;
		}
		void Reset() {
			x = originalX;
			y = originalY;
		}
		inline int moveLeft() {
			x -= 20;
		}
		inline int moveRight() {
			x += 20;
		}
};
class cGameManager {
	private:
		int w, h;
		int offset;
		int bar_w, bar_h;
		int brick_w;
		int brick_h;
		int brick[20][20];
		int file;
		int pause;
		cBall *ball;
		cPaddle *paddle;
		int quit;
		eDir old_dir;
		char wnd[15];
		bool secret;
		string pass;
		int cheat_auto, cheat_instakill, cheat_explosion;
		char cheat;
	public:
		cGameManager(int width = 650, int height = 480,string wnd_name = "Brick Breaker") {
			w = width;
			h = height;
			strcpy(wnd,wnd_name.c_str());
			offset = 10;
			settings>>bar_w>>bar_h;
			//bar_w = 40;
			//bar_h = 30;
			brick_w = (w - (bar_w + offset)) / (bar_w + offset);
			brick_h = (h / 2) / (bar_h + offset);
			ball = new cBall(w / 2,h - 50);
			paddle = new cPaddle(w / 2, h - 35);
			quit = 0;
			pause = 0;
			secret = 0;
			cheat_auto = 0, cheat_instakill = 0, cheat_explosion = 0;
			lvl_builder.clear();
			lvl_builder.seekg(0, std::ios::beg);
			settings.clear();
			settings.seekg(0, std::ios::beg);
			for(int j = 0; j < brick_h; j++) {
				for(int i = 0; i < brick_w; i++) {
					lvl_builder>>file;
					brick[i][j] = file;
				}
			}
			initwindow(w,h,wnd);
			setactivepage(0);
			setvisualpage(1);
		}
		bool checkwin() {
			int sum = 0;
			for(int i = 0; i < brick_w; i++) {
				for(int j = 0; j < brick_h; j++) {
					sum = sum + brick[i][j];
				}
			}
			if(sum == 0) {
				return 1;
			} else {
				return 0;
			}
		}
		void cheats() {
			//auto
			int ballX = ball->getX();
			int ballY = ball->getY();
			int paddleX = paddle->getX();
			if(cheat_auto) {
				if(ballX > paddleX + offset - 2 & ballY > (h - h / 3)) {
					//cout << "Right" << endl;
					paddle->moveRight();
				} else if(ballX < paddleX - offset + 2 & ballY > (h - h / 3)) {
					//cout << "Left" << endl;
					paddle->moveLeft();
				} else {
					//cout << "No action required" << endl;
				}
			}
			//instakill
			//explosion ?
		}
		void Draw() {
			int bar_x = (bar_w + offset);
			int bar_y = (bar_h + offset);
			cleardevice();
			// Edge draw
			bar(0,0,offset,h);
			bar(0,0,w,offset);
			bar(w - offset,0, w, h);
			bar(0, h - offset, w, h);

			// Paddle draw
			int paddleX = paddle->getX();
			int paddleY = paddle->getY();
			bar(paddleX - 25,paddleY,paddleX + 25,paddleY + offset);

			//Ball draw
			int ballX = ball->getX();
			int ballY = ball->getY();
			int ballRad = 10;
			circle(ballX,ballY,ballRad);

			// Guiding X and Y lines for ball
			line(ballX - 15,ballY,ballX + 15,ballY);
			outtextxy(ballX + 25, ballY - 10,"bY");
			line(ballX,ballY - 15,ballX,ballY + 15);
			outtextxy(ballX, ballY + 25,"bX");
			// Guiding X and Y lines for paddle
			line(paddleX - 30,paddleY,paddleX + 30,paddleY);
			outtextxy(paddleX + 25, paddleY - 10,"pY");
			line(paddleX,paddleY - 30,paddleX,paddleY + 30);
			outtextxy(paddleX, paddleY + 25,"pX");

			// Brick draw
			//	lvl_builder.clear();
			//	lvl_builder.seekg(0);
			for(int i = 0; i < brick_w; i++) {
				for(int j = 0; j < brick_h; j++) {
					if(brick[i][j]) {
						bar(i * bar_x + (offset * 2), j * bar_y + (offset * 2), i * bar_x + (offset * 2) + bar_w, j * bar_y + (offset * 2) + bar_h);
						setcolor(RED);
						setlinestyle(SOLID_LINE,1,4);
						line(i * bar_x + (offset * 2),j * bar_y + (offset * 2),i * bar_x + (offset * 2), j * bar_y + (offset * 2) + 40);
						line(i * bar_x + offset * 2, j * bar_y + offset * 2, i * bar_x + offset * 2 + 40, j * bar_y + offset * 2);
						setcolor(WHITE);
						setlinestyle(SOLID_LINE,1,1);
					}
				}
			}
			swapbuffers();
		}
		void Input() {
			ball->Move();
			if(GetAsyncKeyState('A')) {
				paddle->moveLeft();
			}
			if(GetAsyncKeyState('D')) {
				paddle->moveRight();
			}
			if(GetAsyncKeyState('R')) {
				ball->Reset();
				paddle->Reset();
			}
			if(GetAsyncKeyState('Q')) {
				quit = 1;
			}
			if(GetAsyncKeyState('P')) {
				pause = !pause;
				if(pause == 1) {
					old_dir = ball->getDirection();
					ball->changeDirection(STOP);
				} else {
					ball->changeDirection(old_dir);
				}
				delay(50);
			}
			if(GetAsyncKeyState(VK_F1) & 0x8000) {
				cheat_auto = !cheat_auto;
				if(cheat_auto) {
					cout << "Auto Activated" << endl;
				} else {
					cout << "Auto Deactivated" << endl;
				}
				delay(100);
			}
			if(GetKeyState(VK_F2) & 0x8000) {
				cheat_instakill = !cheat_instakill;
				if(cheat_instakill) {
					cout << "Instakill Activated" << endl;
				} else {
					cout << "Instakill Deactivated" << endl;
				}
				delay(100);
			}
		}
		void Logic() {
			int ballX = ball->getX();
			int ballY = ball->getY();
			int paddleX = paddle->getX();
			int paddleY = paddle->getY();
			int ballRad = 10;
			int bar_x = (bar_w + offset);
			int bar_y = (bar_h + offset);

			if(ballX < offset + ballRad) { // left wall
				ball->changeDirection(ball->getDirection() == UPLEFT ? UPRIGHT : DOWNRIGHT);
			}
			if(ballX > w - offset - ballRad) { // right wall
				ball->changeDirection(ball->getDirection() == UPRIGHT ? UPLEFT : DOWNLEFT);
			}
			if(ballY < offset + ballRad) {	// top wall
				ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
			}
			if(ballY > h) { // down wall
				ball->Reset();
				paddle->Reset();
			}

			//Collision bricks (old, bugged - ball goes between bricks)
			/*for(int i = 0; i < brick_w; i++) {
				for(int j = 0; j < brick_h; j++) {
					if(brick[i][j]) {
						if((ballX - ballRad >= i * bar_x + (offset * 2)) & (ballX - ballRad <= i * bar_x + (offset * 2) + bar_w) & (ballY - ballRad >= j * bar_y + (offset * 2)) & (ballY - ballRad <= j * bar_y + (offset * 2) + bar_h)) {
							if(ball->getDirection() == UPLEFT | ball->getDirection() == UPRIGHT) {
								ball->changeDirection(ball->getDirection() == UPLEFT ? UPRIGHT : UPLEFT);
							}
							if(ball->getDirection() == DOWNLEFT | ball->getDirection() == DOWNRIGHT) {
								ball->changeDirection(ball->getDirection() == DOWNLEFT ? DOWNRIGHT : DOWNLEFT);
							}
							brick[i][j] = 0;
						}
					}
				}
			}*/
			// Brick collision
			for(int i = 0; i < brick_w; i++) {
				for(int j = 0; j < brick_h; j++) {
					if(brick[i][j]) {
						// Bottom edge
						if((ballY - ballRad > j * bar_y + (offset * 2) + bar_h + 1 & ballY - ballRad < j * bar_y + (offset * 2) + bar_h + ballRad / 2 + (offset / 2 - 1)) & (ballX > i * bar_x + (offset * 2) & ballX < i * bar_x + (offset * 2) + bar_w)) {
							ball->changeDirection(ball->getDirection() == UPLEFT ? DOWNLEFT : DOWNRIGHT);
							brick[i][j] = 0;
							PlaySoundA((LPCSTR) "..//resources//sounds//Hit_Hurt.wav",NULL,SND_FILENAME | SND_ASYNC );
						}
						// Top edge
						if((ballY + ballRad > j * bar_y + (offset * 2) - 1 & ballY + ballRad < j * bar_y + (offset * 2) + ballRad / 2 + (offset / 2 - 1)) & (ballX > i * bar_x + (offset * 2) & ballX < i * bar_x + (offset * 2) + bar_w)) {
							ball->changeDirection(ball->getDirection() == DOWNLEFT ? UPLEFT : UPRIGHT);
							brick[i][j] = 0;
							PlaySoundA((LPCSTR) "..//resources//sounds//Hit_Hurt.wav",NULL,SND_FILENAME | SND_ASYNC );
						}
						// Left edge
						if((ballX + ballRad > i * bar_x + (offset * 2) - 1 & ballX + ballRad < i * bar_x + (offset * 2) + ballRad / 2 + (offset / 2 - 1)) & (ballY > j * bar_y + (offset * 2) & ballY < j * bar_y + (offset * 2) + bar_h)) {
							ball->changeDirection(ball->getDirection() == DOWNRIGHT ? DOWNLEFT : UPLEFT);
							brick[i][j] = 0;
							PlaySoundA((LPCSTR) "..//resources//sounds//Hit_Hurt.wav",NULL,SND_FILENAME | SND_ASYNC );
						}
						// Right edge
						if((ballX - ballRad > i * bar_x + (offset * 2) + bar_w - 1 & ballX - ballRad < i * bar_x + (offset * 2) + bar_w + ballRad / 2 + (offset / 2 - 1)) & (ballY > j * bar_y + (offset * 2) & ballY < j * bar_y + (offset * 2) + bar_h)) {
							ball->changeDirection(ball->getDirection() == DOWNLEFT ? DOWNRIGHT : UPRIGHT);
							brick[i][j] = 0;
							PlaySoundA((LPCSTR) "..//resources//sounds//Hit_Hurt.wav",NULL,SND_FILENAME | SND_ASYNC );
						}
					}
				}
			}
			// Paddle collision
			if((ballX > paddleX - 25 - 2 & ballX < paddleX + 25 + 2) & ballY > paddleY - 10) { // paddle
				ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
				PlaySoundA((LPCSTR) "..//resources//sounds//Paddle_Bounce.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP );
			}
		}
		int Run() {
			while(!quit) {
				if(GetForegroundWindow() == FindWindowA(NULL,wnd)) {
					Input();
					Logic();
					cheats();
				}
				Draw();
				if(ball->getDirection() == STOP & pause == 0) {
					getch();
					ball->changeDirection(rand() % 51 >= 25 ? UPRIGHT : UPLEFT );
				}
				if(checkwin() == 1) {
					closegraph();
					return 2;
				}
				delay(10);
			}
			return 1;
		}
};

int main(int argc, char** argv) {
	int error = 0; // 0 - unknown; 1 - user quit; 2 - win;
	int width, height;
	settings>>width>>height;
	cGameManager c(width,height,"Brick Breaker");
	error =  c.Run();
	lvl_builder.close();
	settings.close();
	if(error != 0) {
		return error;
	} else {
		return 0;
	}
}