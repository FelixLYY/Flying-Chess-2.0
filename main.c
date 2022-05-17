#include "colortext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct Board {
	char show[15][13];
	int step[15][13];
};

int playerwalk[100];
int playerfinalwalk[100];
int playerturn[100];
int finalterminal[100];
int startwalk[100];
char playername[49] = "R 1R 2R 3R 4Y 1Y 2Y 3Y 4G 1G 2G 3G 4B 1B 2B 3B 4";

struct Board board;

int dice;
int num = 1000;
int number;
void getDice(int num, int i);
void Playermove(int chessnumber);

void declarePlayer();
void chesswalk(int walk[], int n1, int n2, int input);
void AllPlayerBoard(int p, int q);
void checkturn(int chessnumber);
void Wincolour(int win);
void stepother(int chessnumber);
void samecolour(int chessnumber, int finalterminal, int num);
void gamestart();
void NPCmove(int chessnumber);
void gamecase1();
int win;
int countdice = 1;
char yesno;
char colour;
char colourR[9] = "Red";
char colourY[9] = "Yellow";
char colourG[9] = "Green";
char colourB[9] = "Blue";

void DisplayBoard();
void Playboard();
int p, q;
int i, j;
void PrintPlayboard();

void flyingchess(
	int chessnumber1,
	char colour1[],
	int chess2,
	char colour2[],
	int chess3,
	char colour3[],
	int chess4);

int main(void) {
	Playboard();
	declarePlayer();
	printf("\n\n\n");
	/*for(int i =0;i<16;i ++){
	  printf("%d",playerwalk[i]);
	}*/
	gamestart();
	PrintPlayboard();
	return 0;
}

void getDice(int num, int i) {
	time_t t;
	srand((unsigned)time(&t));
	for (int i = 0; i < num; i++) {
		dice = (rand() % 6) + 1;
	}
	printf(
		"%c%c%c roll %d\n",
		playername[i * 3],
		playername[i * 3 + 1],
		playername[i * 3 + 2],
		dice);
}

void checkturn(int chessnumber) {
	if ((playerwalk[chessnumber] > 52) && (playerwalk[chessnumber] < 100)) {
		playerturn[chessnumber] += 1;
		playerwalk[chessnumber] = playerwalk[chessnumber] - 52;
	}
	if ((playerwalk[chessnumber] > playerfinalwalk[chessnumber]) &&
		(playerturn[chessnumber] == 2) && (playerwalk[chessnumber] < 100)) {
		playerwalk[chessnumber] = finalterminal[chessnumber] +
			playerwalk[chessnumber] - playerfinalwalk[chessnumber];
		printf(
			"%c%c%c goes to its final terminal\n",
			playername[chessnumber * 3],
			playername[chessnumber * 3 + 1],
			playername[chessnumber * 3 + 2]);
	}
	if ((playerwalk[chessnumber] > 100) &&
		(playerwalk[chessnumber] % 100 > 6)) {
		playerwalk[chessnumber] = playerwalk[chessnumber] -
			(playerwalk[chessnumber] - finalterminal[chessnumber] - 4 - 1) * 2;
	}
	if ((playerwalk[chessnumber] % 100 == 6) &&
		(playerwalk[chessnumber] > 100)) {
		playerturn[chessnumber] = 3;
		printf(
			"%c%c%c finishs its race \n",
			playername[chessnumber * 3],
			playername[chessnumber * 3 + 1],
			playername[chessnumber * 3 + 2]);
	}
}

void Playermove(int chessnumber) {
	getDice(num, chessnumber);
	if (playerwalk[chessnumber] < 0) {
		if ((dice == 6) || (dice == 5)) {
			if (playerwalk[chessnumber] >= -13) {
				playerwalk[chessnumber] = 46;
				// for red
			} else if (playerwalk[chessnumber] >= -17) {
				playerwalk[chessnumber] = 7;
				// for yellow
			} else if (playerwalk[chessnumber] >= -21) {
				playerwalk[chessnumber] = 33;
				// for green
			} else {
				playerwalk[chessnumber] = 20;
				// for blue
			}
		}
		num += 1;
	} else if (playerturn[chessnumber] != 3) {
		playerwalk[chessnumber] += dice;
		checkturn(chessnumber);
		stepother(chessnumber);
		if (playerwalk[chessnumber] < 53) {
			samecolour(chessnumber, 201, 0);
			samecolour(chessnumber, 301, 3);
			samecolour(chessnumber, 401, 2);
			samecolour(chessnumber, 501, 1);
		}
		stepother(chessnumber);
		checkturn(chessnumber);
	}
	PrintPlayboard();
	sleep(2);
	clrscr();
	if (countdice != 3) {
		if (dice == 6) {
			countdice += 1;
			printf("roll again \n");
			Playermove(chessnumber);
		}
	} else {
		if (dice == 6) {
			printf("Roll three times 6, go back to hangar\n");
			playerwalk[chessnumber] = startwalk[chessnumber];
			PrintPlayboard();
			sleep(2);
			clrscr();
		}
	}
	countdice = 1;
}

void Playboard() {
	int a = 1;
	int row, col;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 13; j++) {
			board.step[i][j] = 0;
		}
	}
	for (row = 3; row < 10; row++) {
		board.step[0][row] = a;
		a++;
	}
	// form step 1 to 7
	for (col = 1; col <= 4; col++) {
		board.step[col][9] = a;
		a++;
	}
	// form step 8 to 11
	for (row = 10; row < 13; row++) {
		board.step[4][row] = a;
		a++;
	}
	// form step 12 to 14
	for (col = 5; col <= 10; col++) {
		board.step[col][12] = a;
		a++;
	}
	// form step 15 to 20
	for (row = 11; row >= 9; row--) {
		board.step[10][row] = a;
		a++;
	}
	// form step 21 to 23
	for (col = 11; col < 15; col++) {
		board.step[col][9] = a;
		a++;
	}
	// form step 24 to 27
	for (row = 8; row >= 3; row--) {
		board.step[14][row] = a;
		a++;
	}
	// form step 28 to 33
	for (col = 13; col >= 10; col--) {
		board.step[col][3] = a;
		a++;
	}
	// form step 34 to 37
	for (row = 2; row >= 0; row--) {
		board.step[10][row] = a;
		a++;
	}
	// form step 38 to 40
	for (col = 9; col >= 4; col--) {
		board.step[col][0] = a;
		a++;
	}
	// form step 41 to 46
	for (row = 1; row <= 3; row++) {
		board.step[4][row] = a;
		a++;
	}
	// form step 47 to 49
	for (col = 3; col > 0; col--) {
		board.step[col][3] = a;
		a++;
	}
	// form step 50 to 52
	a = 1;
	for (col = 1; col <= 4; col++) {
		board.step[col][6] = 200 + a;
		a++;
	}
	board.step[5][6] = 220;
	board.step[6][6] = 205;
	// Final terminal for the yellow chess
	a = 1;
	for (row = 1; row <= 5; row++) {
		board.step[7][row] = 300 + a;
		a++;
	}
	// Final terminal for the red chess
	a = 1;
	for (col = 13; col >= 10; col--) {
		board.step[col][6] = 400 + a;
		a++;
	}
	board.step[9][6] = 440;
	board.step[8][6] = 405;
	// Final terminal for the green chess
	a = 1;
	for (row = 11; row >= 7; row--) {
		board.step[7][row] = 500 + a;
		a++;
	}
	// Final terminal for the blue chess
	board.step[7][6] = 601;
	//"End" at the centre
	board.step[1][1] = -1;
	// Write R
	board.step[1][11] = -2;
	// Write Y
	board.step[13][1] = -3;
	// Write G
	board.step[13][11] = -4;
	// Write B
	board.step[0][0] = -10;
	board.step[0][2] = -11;
	board.step[2][0] = -12;
	board.step[2][2] = -13;

	board.step[0][10] = -14;
	board.step[0][12] = -15;
	board.step[2][10] = -16;
	board.step[2][12] = -17;

	board.step[12][0] = -18;
	board.step[12][2] = -19;
	board.step[14][0] = -20;
	board.step[14][2] = -21;

	board.step[12][10] = -22;
	board.step[12][12] = -23;
	board.step[14][10] = -24;
	board.step[14][12] = -25;
}

void declarePlayer() {
	for (int i = 0; i < 16; i++) {
		startwalk[i] = -10 - i;
		playerwalk[i] = startwalk[i];
		playerturn[i] = 0;
	}
	chesswalk(playerfinalwalk, 0, 4, 43);
	chesswalk(playerfinalwalk, 4, 8, 4);
	chesswalk(playerfinalwalk, 8, 12, 30);
	chesswalk(playerfinalwalk, 12, 16, 17);

	chesswalk(finalterminal, 0, 4, 301);
	chesswalk(finalterminal, 4, 8, 201);
	chesswalk(finalterminal, 8, 12, 401);
	chesswalk(finalterminal, 12, 16, 501);
}

void chesswalk(int walk[], int n1, int n2, int input) {
	for (int i = n1; i < n2; i++) {
		walk[i] = input;
	}
}

void AllPlayerBoard(int p, int q) {
	int returnvalue = 0;
	for (int i = 0; i < 16; i++) {
		if (board.step[p][q] == playerwalk[i]) {
			printf(
				"%c%c%c",
				playername[i * 3],
				playername[i * 3 + 1],
				playername[i * 3 + 2]);
			returnvalue = 1;
		}
	}
	if (returnvalue == 0) {
		printf("   %c", board.show[p][q]);
	}
}

void PrintPlayboard() {
	for (p = 0; p < 15; p++) {
		for (q = 0; q < 13; q++) {
			if (board.step[p][q] > 0) {
				if (board.step[p][q] < 170) {
					if (board.step[p][q] % 4 == 1) {
						// printf("%s",Black);
						printf("%s", On_Blue);
						AllPlayerBoard(p, q);
					} else if (board.step[p][q] % 4 == 2) {
						printf("%s", Black);
						printf("%s", On_IGreen);
						AllPlayerBoard(p, q);
					} else if (board.step[p][q] % 4 == 3) {
						// printf("%s",Black);
						printf("%s", On_Red);
						AllPlayerBoard(p, q);
					} else {
						printf("%s", Black);
						printf("%s", On_IYellow);
						AllPlayerBoard(p, q);
					}

				} else {
					if (board.step[p][q] > 600) {
						reset();
						printf("END");
					} else if (board.step[p][q] > 500) {
						printf("%s", Black);
						printf("%s", On_IBlue);
						AllPlayerBoard(p, q);

					} else if (board.step[p][q] > 400) {
						printf("%s", Black);
						printf("%s", On_IGreen);
						AllPlayerBoard(p, q);
					} else if (board.step[p][q] > 300) {
						// printf("%s",Red);
						printf("%s", On_IRed);
						AllPlayerBoard(p, q);
					} else if (board.step[p][q] > 200) {
						printf("%s", Black);
						printf("%s", On_IYellow);
						AllPlayerBoard(p, q);
					}
				}
			} else if (board.step[p][q] == -1) {
				reset();
				printf(" R ");
			} else if (board.step[p][q] == -2) {
				reset();
				printf(" Y ");
			} else if (board.step[p][q] == -3) {
				reset();
				printf(" G ");
			} else if (board.step[p][q] == -4) {
				reset();
				printf(" B ");
			} else if (board.step[p][q] < -5) {
				reset();
				AllPlayerBoard(p, q);
			} else {
				reset();
				printf("   ");
			}
			if (q == 12) {
				printf("\n");
			}
		}
	}
}

void Wincolour(int win) {
	for (int i = 0; i < 4; i++) {
		if ((playerturn[i * 3] == 3) && (playerturn[i * 3 + 1] == 3) &&
			(playerturn[i * 3 + 2] == 3) && (playerturn[i * 3 + 3] == 3)) {
			if (i == 0) {
				printf("Red wins the game!\n");
			} else if (i == 1) {
				printf("Yellow wins the game!\n");
			} else if (i == 2) {
				printf("Green wins the game!\n");
			} else if (i == 3) {
				printf("Blue wins the game!\n");
			}
			win = 1;
		}
	}
	if (win == 1) {
		printf("Do you want to play again?");
		printf("Type \"Y\" to go back to menu");
		scanf("%c", &yesno);
		if (yesno == 'Y') {
			gamestart();

		} else {
			exit(0);
		}
	}
}

void stepother(int chessnumber) {
	for (int i = 0; i < 16; i++) {
		if (i != chessnumber) {
			if (playerwalk[chessnumber] == playerwalk[i]) {
				playerwalk[i] = startwalk[i];
				playerturn[i] = 0;
				printf(
					"%c%c%c steps on %c%c%c, %c%c%c go back to hangar\n",
					playername[chessnumber * 3],
					playername[chessnumber * 3 + 1],
					playername[chessnumber * 3 + 2],
					playername[i * 3],
					playername[i * 3 + 1],
					playername[i * 3 + 2],
					playername[i * 3],
					playername[i * 3 + 1],
					playername[i * 3 + 2]);
			}
		}
	}
}

void samecolour(int chessnumber, int confirmvalue, int num) {
	if (finalterminal[chessnumber] == confirmvalue) {
		if (playerwalk[chessnumber] % 4 == num) {
			printf(
				"%c%c%c step on same colour!\n",
				playername[chessnumber * 3],
				playername[chessnumber * 3 + 1],
				playername[chessnumber * 3 + 2]);
			PrintPlayboard();
			sleep(2);
			clrscr();
			playerwalk[chessnumber] += 4;
			printf(
				"%c%c%c go 4 more steps further!\n",
				playername[chessnumber * 3],
				playername[chessnumber * 3 + 1],
				playername[chessnumber * 3 + 2]);
		}
	}
}

void NPCmove(int chessnumber) {
	if (playerturn[chessnumber + 3] != 3) {
		if (playerturn[chessnumber + 2] != 3) {
			if (playerturn[chessnumber + 1] != 3) {
				if (playerturn[chessnumber] != 3) {
					Playermove(chessnumber);

				} else {
					Playermove(chessnumber + 1);
				}
			} else {
				Playermove(chessnumber + 2);
			}
		} else {
			Playermove(chessnumber + 3);
		}
	}
}

void flyingchess(
	int chessnumber1,
	char colour1[],
	int chess2,
	char colour2[],
	int chess3,
	char colour3[],
	int chess4) {
	do {
		clrscr();
		printf("Time for you to roll\n");
		printf("Which chess do you want to move?\n");
		printf(
			"\"%c%c%c\" , \"%c%c%c\" , \"%c%c%c\" , or \"%c%c%c\"\nType \"1\" "
			"for %c%c%c , \"2\" for %c%c%c, \"3\" for %c%c%c \"4\" for "
			"%c%c%c\n",
			playername[chessnumber1],
			playername[chessnumber1 + 1],
			playername[chessnumber1 + 2],
			playername[chessnumber1 + 3],
			playername[chessnumber1 + 4],
			playername[chessnumber1 + 5],
			playername[chessnumber1 + 6],
			playername[chessnumber1 + 7],
			playername[chessnumber1 + 8],
			playername[chessnumber1 + 9],
			playername[chessnumber1 + 10],
			playername[chessnumber1 + 11],
			playername[chessnumber1],
			playername[chessnumber1 + 1],
			playername[chessnumber1 + 2],
			playername[chessnumber1 + 3],
			playername[chessnumber1 + 4],
			playername[chessnumber1 + 5],
			playername[chessnumber1 + 6],
			playername[chessnumber1 + 7],
			playername[chessnumber1 + 8],
			playername[chessnumber1 + 9],
			playername[chessnumber1 + 10],
			playername[chessnumber1 + 11]);
		printf("Type others to automatic play\n");
		PrintPlayboard();
		scanf("%d", &number);
		clrscr();
		if (number == 1) {
			Playermove(chessnumber1);
		} else if (number == 2) {
			Playermove(chessnumber1 + 1);
		} else if (number == 3) {
			Playermove(chessnumber1 + 2);
		} else if (number == 4) {
			Playermove(chessnumber1 + 3);
		} else {
			printf("Wrong Input, Please Type again");
			clrscr();
			flyingchess(
				chessnumber1,
				colour1,
				chess2,
				colour2,
				chess3,
				colour3,
				chess4);
		}
		Wincolour(win);
		printf("Time for %s to go\n", colour1);
		NPCmove(chess2);
		Wincolour(win);
		printf("Time for %s to go\n", colour2);
		NPCmove(chess3);
		Wincolour(win);
		printf("Time for %s to go\n", colour3);
		NPCmove(chess4);
		Wincolour(win);

	} while (win == 0);
}

void gamecase1() {
	printf("Here game starts\n");
	printf("Which colour do you want to be?\n");
	printf("Type \"R\" if you want to use red chess\nType \"Y\" if you want to "
		   "use yellow chess\nType \"G\" if you want to use green chess\nType "
		   "\"B\" if you want to use blue chess\nType \"E\" if you want to go "
		   "back to menu\n");
	scanf("%s", &colour);
	switch (colour) {
	case 'R':
		flyingchess(0, colourY, 4, colourG, 8, colourB, 12);
		break;
	case 'Y':
		flyingchess(4, colourG, 8, colourB, 12, colourR, 0);
		break;
	case 'G':
		flyingchess(8, colourB, 12, colourR, 0, colourY, 4);
		break;
	case 'B':
		flyingchess(12, colourR, 0, colourY, 4, colourG, 8);
		break;
	case 'E':
		gamestart();
		break;

	default:
		printf("Wrong Input\n");
		printf("Please select again from menu\n");
		gamecase1();
		break;
	}
}

void gamestart() {
	int num;
	char word[100];
	printf("\t\t Welcome to Flying Chess!\n");
	printf("1. Start\n2. Rules\n3. Exit \n");
	printf("Type \"1\" for starting the game\nType \"2\" to read the "
		   "rules\nType \"3\" to exit\n");
	scanf("%d", &num);
	switch (num) {
	case 1:
		gamecase1();
		break;
	case 2:
		printf("Here are the rules\n");
		printf("1.You can move a chess out onto the board if you roll 6\n");
		printf("2. If you roll 5 or 6, you can roll again. If you roll 6 "
			   "again, you can roll again. If you roll 6 for three times, the "
			   "chess you moved go back to the hangar.\n");
		printf("3. When a player lands on a chess, that chess returns that "
			   "piece to its hangar.\n");
		printf("4. When a plane lands on a space of its own colour, it "
			   "immediately jumps to the next space of its own colour.  \n");
		printf("If you want to go back, type anything\n");
		scanf("%s", word);
		clrscr();
		gamestart();
		break;
	case 3:
		printf("You exit");
		break;

	default:
		printf("Wrong input, please input again\n");
		sleep(2);
		clrscr();
		gamestart();
		break;
	}
}
