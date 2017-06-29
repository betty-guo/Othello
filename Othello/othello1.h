// Othello1.h : Defines the entry point for the console application.
//Betty Guo
//ICS 3UI Summative
//17/06/15
//Mr Damian

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Score//used to hold player profile info
{
	string name = " ";//name of player
	int score = 0;//number of pieces on board by end
};

struct leaderboard//used to read and print from leaderboard
{
	string name = " ";//name of player
	int win = 0;//number of wins
	int loss = 0;//number of losses
};

class Board
{
public:
	Board();
	Score player1;//p1 player profile
	Score player2;//p2 player profile
	int w, b;//piece counter for each player
	int mode;
	void reset();//reset function wipes board and all values
	bool sent;//player move marker
	vector<int> Moves{ 0 };//stack holding moves made at each turn
	bool go = false;//gameover sentinel
	int level=1;//AI depth search 1= easy, 2 =medium, 3=hard
	int moveCount = 1;//stack counter for Moves
	int Board1[1600] = { 0 };//board used for minimax algorithm simulations
	int BestPos;//best position variable
	int DisplayBoard[100] = { 0 };//display board used to output onto screen
	bool PlayerCanMove = true;
	
	void updateBoard(void);//initializes first pieces
	void dupeBoard(int);//creates new level for simulation
	bool validMove(int, int, int, int);//checks if square is valid move for certain color
	int search(int, int, int, int, bool, int, int);//minimax algorithm, searches for best move (recursive) at each level
	bool displayBoard(bool);//prints to screen
	void animateMove(int, int, int);//step one of animating move
	void animateStep(int, int, int, int, int);//step two of animating move
	void playerMove(int, int);//player input to launch animations
	void playerMoveDone1(int);//step three of animating move
	void playerMoveDone2(void);//step four of animating move, launches minimax algorithm
	void playerMoveDone3(int);//gameover check
	int readHighScore(void);//read score
	int writeHighScore(void);//write score
	void printHighScore(void);//print score
};

int stringParse(string);
class nonumber : public runtime_error//exception thrown for bad input
{
public:
	nonumber(string k);
};

