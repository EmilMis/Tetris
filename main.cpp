#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>

#define pc vector<vector<int>>

using namespace std;

const int x = 10;
const int y = 20;

int b[y][x];

char block_char = 'O';

int game_over = 0;

pc I = { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
pc L = { {0, 0}, {0, 1}, {0, 2}, {1, 2} };
pc J = { {1, 0}, {0, 0}, {0, 1}, {0, 2} };
pc T = { {0, 0}, {0, 1}, {0, 2}, {1, 1} };
pc S = { {0, 2}, {0, 1}, {1, 1}, {1, 0} };
pc Z = { {0, 0}, {0, 1}, {1, 1}, {1, 2} };
pc O = { {0, 0}, {0, 1}, {1, 1}, {1, 0} };

vector<pc> pieces = { I, L, J, T, S, Z, O };

int display_n = 0;
pc pc_display;

void set_board() {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			b[i][j] = 0;
		}
	}
}
void reset() {
	printf("\x1b[%dD", x + 2);
	printf("\x1b[%dA", y + 2);
}
void display(vector<vector<int>> fall) {
	for (int i = 0; i < x + 2; i++) {
		fputc('#', stdout);
	}
	fputc('\n', stdout);
	for (int i = 0; i < y; i++) {
		fputc('#', stdout);
		char r[x];
		for (int j = 0; j < x; j++) {
			int c = 1;
			for (int n = 0; n < fall.size(); n++) {
				if (fall[n][0] == i && fall[n][1] == j) {
					r[j] = block_char;
					c = 0;
					break;
				}
			}
			if (c == 0) {
				continue;
			}
			if (b[i][j] == 1) {
				r[j] = block_char;
			}
			else {
				r[j] = ' ';
			}
		}
		fwrite(r, 1, x, stdout);
		fputc('#', stdout);
		fputc('\n', stdout);
	}
	for (int i = 0; i < x + 2; i++) {
		fputc('#', stdout);
	}
	fputc('\n', stdout);
}
void sleep(int ms) {
	this_thread::sleep_for(chrono::milliseconds(ms));
}
void clear_line(vector<int> ind) {
	for (int i = 0; i < x / 2 + 1; i++) {
		for (int j = 0; j < ind.size(); j++) {
			b[ind[j]][x / 2 - 1 + i] = 0;
			b[ind[j]][x / 2 - i] = 0;
		}
		display({});
		sleep(40);
		reset();
	}
	for (int i = 0; i < ind.size(); i++) {
		for (int j = ind[i]; j > 0; j--) {
			if (j == 0) {
				continue;
			}
			for (int n = 0; n < x; n++) {
				b[j][n] = b[j - 1][n];
				b[j - 1][n] = 0;
			}
		}
	}
}
int random_n(int s, int l) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> dist(s, l);
	return dist(rng);
}
void fall(int pic) {
	int xp = x / 2 - 1;
	int yp = 0;
	for (int i = 0; i < y + 1; i++) {
		pc piece = pieces[pic];
		for (int j = 0; j < 4; j++) {
			piece[j][0] += yp;
			piece[j][1] += xp;
		}
		for (int j = 0; j < 4; j++) {
			vector<int> pxpy = piece[j];
			if (pxpy[0] == y - 1 || b[pxpy[0] + 1][pxpy[1]] == 1) {
				for (int n = 0; n < 4; n++) {
					vector<int> pxpyb = piece[n];
					b[pxpyb[0]][pxpyb[1]] = 1;
				}
				pc_display = {};
				display_n = 1;
				sleep(50);
				if (yp == 0) {
					game_over = 1;
				}
				return;
			}
		}
		pc_display = piece;
		display_n = 1;
		sleep(50);
		yp++;
	}
}
void game_over_() {
	cout << R"(
   ______                        ____ _    ____________     
  / ____/___ _____ ___  ___     / __ \ |  / / ____/ __ \    
 / / __/ __ `/ __ `__ \/ _ \   / / / / | / / __/ / /_/ /    
/ /_/ / /_/ / / / / / /  __/  / /_/ /| |/ / /___/ _, _/     
\____/\__,_/_/ /_/ /_/\___/   \____/ |___/_____/_/ |_|      
						)";
}

void display_state() {
	while (true) {
		if (display_n) {
			display_n = 0;
			reset();
			display(pc_display);
		}
	}
}

int main(void) {
	thread th(display_state);
	for (int i = 0; i < 20; i++) {
		fall(random_n(0, 6));
		if (game_over) {
			game_over_();
			return 0;
		}
	}
	display({});
}
