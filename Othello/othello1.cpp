// Othello1.cpp : Defines the entry point for the console application.

#include "othello1.h"
#include <string>
#include <vector>
#include <fstream>//file stream
#include <algorithm>//includes sort
#include <iomanip>//setw


int Directions[] = { -1, -11, -10, -9, 1, 11, 10, 9 };//for search function, values that represent moving in certain directions
int Corners[] = { 11, 18, 81, 88 };//for search function, values representing corners
int NearCorners[] = { 22, 27, 72, 77 };//for search function, values representing near corners

Board::Board(void) {//constructor sets virtual board base 1 (board with moves made by player) to empty square = 0 or outside board = 3
	for (int i = 0; i < 1600; i += 100) {
		for (int j = 0; j < 100; j++) {
			if (j < 11 || j > 88 || (j + 1) % 10 < 2) {//for anything outside the board
				Board1[i + j] = 3;
			}
			else {//if inside the board
				Board1[i + j] = 0;
			}
		}
	}
}

void Board::updateBoard(void)//initializes board with four pieces in the middle (used with reset function)
{
	for (int i = 0; i < 100; i++) {
		if (i < 11 || i > 88 || (i + 1) % 10 < 2) {
			Board1[i] = 3;
		}
		else {
			Board1[i] = 0;
		}
	}
	Board1[44] = Board1[55] = 1;//set two pieces in middle to white
	Board1[45] = Board1[54] = 2;//set two other pieces in middle to black

	int color = 2;//
	Moves.resize(moveCount+1);
	for (int m = 0; m < moveCount+1; m++) {//
		int p = Moves[m];
		if (p != 0){
			validMove(0, p, color, 0);
		}
		color = 3 - color;
	}
	moveCount++;

}

void Board::dupeBoard (int base)
{
	int p = base + 111;//simulates moves on "new board"
	for (int i = base + 11; i < base + 89; i++) {// squares 11-88 on new base
		Board1[p++] = Board1[i];//set new board equal to board level beneath (already simulated one with best move in that case)
	}
}

bool Board::validMove (int base, int move, int color, int delta)
{
	int p = base + move;
	if (Board1[p] != 0) {
		return false;
	}
	int other = 3 - color;
	bool ok = false;//validmove indicator
	for (int d = 7; d >= 0; d--) {
		int dd = Directions[d];//scans in every direction for block of different color
		int n = p + dd;//travel in that direction on the level of simulation
		while (Board1[n] == other) {//while the board square is of another block
			n += dd;//travel in that direction
		}//stops when block of same color/ empty block is found
		if (Board1[n] == color && (n - dd != p)) { //if block found is player's color and its not the starting block
			if (!ok) {//if indicator says no
				ok = true;//say yes
				dupeBoard(base);//create a dupeboard for AI based off base 0 board (current one)
			}
			while (n != p) {//while not back at original square
				n -= dd;//go back in direction scanned from
				Board1[n + delta] = color;//change blocks to player color
			}
		}
	}
	return ok;
}

int Board::search (int d, int base, int color, int depth, bool flag, int minscore, int maxscore)//recursive minimax function
{
	int other = 3 - color;//sets other to 3 minus current color
	if (d > depth) {//if level of simulation larger than depth
		int s = 0;
		for (int c = 0; c < 4; c++) {//check four corners
			int q = Board1[base + Corners[c]];
			if (q == color) {//if corner is players color
				s += 300;//add to score of move
			}
			else if (q == other) {//if its other players piece
				s -= 300;//lower score of move
			}
			else {//if no piece in corner
				q = Board1[base + NearCorners[c]];//check places near corner
				if (q == color) {//if its the current player's piece
					s -= 50;//lower score
				}
				else if (q == other) {//if its the other player's piece
					s += 50;//raise score
				}
			}
		}
		return s;//return score
	}
	int best = 0;//reset best move to zero
	int bestscore = -9000;//reset bestscore to minimum score (worst move possible)
	if (d < depth - 1) {//if its not final depth layer
		bestscore = minscore;//set bestscore to minimum score
	}
	int n = 0;//counter for amount of moves possible, if none then return 0
	for (int p = 11; p < 89; p++) {//squares 11-88
		if (validMove(base, p, color, 100)) {//test if valid move for color on square
			n++;
			if (level == 1)//if depth of one
			{
				BestPos = p;//return best position is first valid move
				return 1;
			}
			int ss = -(search(d + 1, base + 100, other, depth, false, -maxscore, -bestscore));//invert sign of negative of best score
			if (ss > bestscore) {
				best = BestPos = p;//found best square if the score is higher than best possible score found so far
				bestscore = ss;//set best score to score if placed at that square

				if (ss >= maxscore || ss >= 8003) {//if ss is close or equal to the max score (best move) then return that as best score
					return ss;
				}
			}
		}
	}
	if (n == 0) {//no moves
		best = 0;
		if (flag) {
			for (int z = base + 11; z < base + 89; z++) {
				if (Board1[z] == color) {//search on that level if block is color of player
					n++;//number of moves add
				}
				else if (Board1[z] == other) {
					n--;
				}
			}
			if (n > 0) {
				return n + 8000;
			}
			else {
				return n - 8000;
			}
		}
		dupeBoard(base);
		bestscore = -(search(d + 1, base + 100, other, depth, true, -maxscore, -bestscore));
	}
	BestPos = best;
	if (d >= depth - 1) {
		return bestscore + (n << 3);//returns best score with binary
	}
	else {
		return bestscore;//return best score if searched to depth of level
	}
}

void Board::reset()
{
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			DisplayBoard[(y + 1) * 10 + (x + 1)] = 0;
		}
	}
	Moves.clear();
	updateBoard();
	displayBoard(true);
}

bool Board::displayBoard (bool markers)
{
	system("cls");//clear screen for animation
	PlayerCanMove = markers;
	bool any = false;//any moves available
	int white = 0;
	int black = 0;
	int img;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			int move = (y + 1) * 10 + x + 1;
			switch (Board1[move]) {
			case 0:
				if (validMove(0, move, 2, 100)) {
					any = true;
				}
				img = 0;
				break;
			case 1:
				img = 1;
				white++;//adds to number of white pieces
				break;
			case 2:
				img = 2;
				black++;//adds to number of black pieces
				break;
			case 3:
				img = 3;
				break;
			}
			DisplayBoard[move] = img;//sets DisplayBoard equal to Board1 first level
		}
	}
		cout << " "; //output starts
		for (int j = 0; j < 8; ++j)
		{
			cout << "  " << char('A' + j) << " ";
		}
		cout << endl;
		cout << " ";
		for (int j = 0; j < 8; ++j)
		{
			cout << "+---";
		}
		cout << "+" << endl;

		for (int i = 0; i < 8; ++i)
		{
			cout << i + 1;
			for (int j = 0; j < 8; ++j)
			{
				char f = ' ';

				int move = (i + 1) * 10 + j + 1;
				switch (DisplayBoard[move]) {
				case 0:
					break;
				case 1:
					f = (char)219;//output white
					break;
				case 2:
					f = (char)176;//ouptut black
					break;
				}
				cout << "| " << f << " ";
			}
			cout << "|" << endl;
			cout << " ";
			for (int j = 0; j < 8; ++j)
			{
				cout << "+---";
			}
			cout << "+" << endl;
		}
		
		cout << "White: " << white << endl;//print number of pieces
		cout << "Black: " << black << endl;
		player1.score = white;
		player2.score = black; //sets global variables equal to number of pieces
	return any;
}


void Board::animateMove (int move, int color, int function)
{
	if (move == 0 && function == 1 ) {
		playerMoveDone1(move);//launches AI move
		return;
	}
	if (move == 0 && function == 3) {
		playerMoveDone3(color);//checks for gameover state
		return;
	}
	switch (color) {
	case 1:
		Board1[move] = 1;
		break;
	case 2:
		Board1[move] = 2;
		break;
	}
	displayBoard(false);
	animateStep(function, move, color, 0, 0);
}

void Board::animateStep (int whenDone, int move, int color, int direction, int distance)
{
	if (direction >= 8 && whenDone == 1) {//launches AI
		playerMoveDone1(move);
		return;
	}
	if (direction >= 8 && whenDone == 3) {
		playerMoveDone3(color); //tests for game over
		return;
	}
	int other = 3 - color;
	if (distance == 0) {
		if (Board1[move + Directions[direction]] != other) {
			animateStep(whenDone, move, color, direction + 1, 0);
			return;
		}
		int i = 2;
		while (Board1[move + i*Directions[direction]] == other) {
			i++;
		}
		if (Board1[move + i*Directions[direction]] != color) {
			animateStep(whenDone, move, color, direction + 1, 0);
			return;
		}
		distance = 1;
	}
	if (Board1[move + distance*Directions[direction]] != other) {
		animateStep(whenDone, move, color, direction + 1, 0);
		return;
	}
	int img;
	switch (color) {
	case 1:
		img = 1;
		break;
	case 2:
		img = 2;
		break;
	}
	Board1[move + distance*Directions[direction]] = img;
	animateStep(whenDone, move, color, direction, distance + 1);
}

void Board::playerMove(int move, int color)
{
	if (!PlayerCanMove) {
		return;
	}
	bool any = displayBoard(false);
	if (move == 0) {
		if (any) {
			displayBoard(true);
			cout << "You may not pass if you have a valid move." << endl;
			sent = true;
			return;
		}
	}
	else if (mode == 1){ 
		if (!validMove(0, move, 2, 100)) {
			displayBoard(true);
			cout << "That move is not valid." << endl;
			return;
		}
		animateMove(move, 2, 1);
	} 
	else if (mode==2)
	{
		if (!validMove(0, move, color, 100)) {
			displayBoard(true);
			cout << "That move is not valid." << endl;
			return;
		}
		animateMove(move, color, 1);
	}
}
void Board::playerMoveDone1(int move)
{
	validMove(0, move, 2, 0);
	Moves.resize(moveCount + 1);//help throws error???
	Moves[moveCount] = move;
	moveCount++;
	displayBoard(false);//prints board without allowing player to enter input
	if (mode == 1) {//if 1 player mode
		playerMoveDone2();//launches AI
	}
}
void Board::playerMoveDone2(void)
{
	search(0, 0, 1, level, false, -9000, 9000);//search for best move using depth of level 1, 2, 3 depending on level (depth is how many moves it looks ahead)
	animateMove(BestPos, 1, 3);//animate Best Move
}
void Board::playerMoveDone3(int color)
{
	validMove(0, BestPos, color, 0);
	Moves.resize(moveCount + 1);
	Moves[moveCount] = BestPos;
	moveCount++;
	bool any = displayBoard(true);
	if (!any) {
		bool gameover = true;
		for (int y = 0; y < 8 && gameover; y++) {
			for (int x = 0; x < 8 && gameover; x++) {
				if (validMove(0, (y + 1) * 10 + x + 1, color, 100)) {
					gameover = false;
				}
			}
		}
		if (gameover) {
			go = true;//gameover sentinel
			cout << "Game over." << endl;
			if (player1.score>player2.score)
			{
				cout << "White wins." << endl;//outputs winner
			}
			else if (player2.score>player1.score)
			{
				cout << "Black wins." << endl;
			}
			else
			{
				cout << "Draw." << endl;
			}
		}
	}
}

nonumber::nonumber(string k) : runtime_error(k) {
}

int stringParse(string in)
{
	int w = 0;//errorproofing
	string b;
		if (in[0] == '0')
		{
			return w;
		}
		if (((in[1] >= 'A' && in[1] <= 'H') || (in[1] >= 'a' && in[1] <= 'h') )&& (in[0] >= '1' && in[0] <= '8')) {
			b = in[0];
			w = stoi(b) * 10;
			switch(in[1])
			{
			case 'A':
				w += 1;
				break;
			case 'B':
				w += 2;
				break;
			case 'C':
				w += 3;
				break;
			case 'D':
				w += 4;
				break;
			case 'E':
				w += 5;
				break;
			case 'F':
				w += 6;
				break;
			case 'G':
				w += 7;
				break;
			case 'H':
				w += 8;
				break;
			case 'a':
				w += 1;
				break;
			case 'b':
				w += 2;
				break;
			case 'c':
				w += 3;
				break;
			case 'd':
				w += 4;
				break;
			case 'e':
				w += 5;
				break;
			case 'f':
				w += 6;
				break;
			case 'g':
				w += 7;
				break;
			case 'h':
				w += 8;
				break;
			}
		}
		else {
			throw nonumber("This is not a valid input");
		}

		return w;
}

vector <leaderboard> scores(12);//array for scores up to 12 scores (on scoreboard with two new names)
int lineCount = 0;//counter for amount of people on scoreboard

int Board::readHighScore(void)//read from highscore file
{
	ifstream ifs;
	ifs.open("Scores.txt", ifstream::in);//opens score file for reading

	if (!ifs) {
		cout << "Cannot open input file.\n";
		return 1;
	}

	string m;//variable to hold text file info

	while (getline(ifs, m)) {//while there are lines in the highscore file
		string q = "";//substring holder
		for (unsigned int i = 0; i < m.length(); i++)
		{
			if (m[i] == '|')//if hits sentinel save previous substring as name
			{
				scores.at(lineCount).name = q;
				q.clear();
			}
			if (m[i] == '%')//if hits sentinel save previous substring as number of wins
			{
				scores.at(lineCount).win = stoi(q);
				q.clear();
			}
			if (m[i] == '*')//if hits sentinel save previous substring as score
			{
				scores.at(lineCount).loss = stoi(q);
				q.clear();
			}
			if (m[i] != '|' && m[i] != '%' && m[i] != '*') {
				q += m[i];//reads string into substring
			}
		}
		lineCount++;//increase line counter
	}
	ifs.close();//closes file
	writeHighScore();
	/*scores.at(10).name = player1.name;
	scores.at(10).score = player1.score;
	printHighScore();*/
	return 0;
}

struct myclass {//sorting class
	bool operator() (leaderboard i, leaderboard j) { 
		if (i.win == 0 && j.win == 0)//if no wins
		{
			return (i.loss > j.loss);//rank by lowest zero
		}
		else if (i.win == 0)//if i has no wins
		{
			return (true);//return that i<j = true
		}
		else if (j.win == 0)//if j has no wins
		{
			return (false);//return that i>j = true
		}
		else if (i.loss == 0 && j.loss == 0)//if no loss
		{
			return (i.win > j.win);//rank by lowest zero
		}
		else if (i.loss == 0)//if i has no wins
		{
			return (i.win < j.win/j.loss);//return that i<j = true
		}
		else if (j.loss == 0)//if j has no wins
		{
			return (i.win/i.loss<j.win);//return that i>j = true
		}
		return ((i.win/i.loss)<(j.win/j.loss)); 
	}
} myobject;

int Board::writeHighScore(void)//sort and write new highscore from game onto file
{
	if (mode == 2 && player1.score > player2.score) //if two player and white wins
	{
		bool onBoard = false;//check if winner name is onBoard
		for (int i = 0; i< lineCount; i++)
		{
			if (scores.at(i).name == player1.name)
			{
				scores.at(i).win += 1;
				onBoard = true;
			}
		}
		if (!onBoard)//if not on board then output as new score thing
		{
			scores.at(lineCount).name = player1.name;
			scores.at(lineCount).win += 1;
			lineCount++;//increase amount of lines on scoreboard (when used with array includes newly added person in array)
		}
		
		onBoard = false;//check if loser name is onBoard
		for (int i = 0; i< lineCount-1; i++)
		{
			if (scores.at(i).name == player2.name)
			{
				scores.at(i).loss += 1;
				onBoard = true;
			}
		}
		if (!onBoard)//if not on board then output as new score thing
		{
			scores.at(lineCount++).name = player2.name;
			scores.at(lineCount++).loss += 1;
			lineCount++;//increase amount of lines on scoreboard (when used with array includes newly added person in array)
		}

	}
	else if (mode == 2 && player2.score > player1.score) //if two player and black wins
	{
		bool onBoard = false;//check if winner name is onBoard
		for (int i = 0; i< lineCount; i++)
		{
			if (scores.at(i).name == player2.name)
			{
				scores.at(i).win += 1;
				onBoard = true;
			}
		}
		if (!onBoard)//if not on board then output as new score thing
		{
			scores.at(lineCount).name = player2.name;
			scores.at(lineCount).win += 1;
			lineCount++;//increase amount of lines on scoreboard (when used with array includes newly added person in array)
		}

		onBoard = false;//check if loser name is onBoard
		for (int i = 0; i< lineCount - 1; i++)
		{
			if (scores.at(i).name == player1.name)
			{
				scores.at(i).loss += 1;
				onBoard = true;
			}
		}
		if (!onBoard)//if not on board then output as new score thing
		{
			scores.at(lineCount++).name = player1.name;
			scores.at(lineCount++).loss += 1;
			lineCount++;//increase amount of lines on scoreboard (when used with array includes newly added person in array)
		}
	}
	//NOTE: No scores are saved in the case of a draw
	
	//SORTING ----------------------------------------------------------------------------------------

	// using object as comp
	std::sort(scores.begin(), scores.begin()+(lineCount), myobject);//sorts from largest to smallest based on ratio of wins to losses

	//SORTING ENDS------------------------------------------------------------------------------------
	
	printHighScore();//prints onto screen
	//WRITING INTO FILE-------------------------------------------------------------------------------

	ofstream ofs;
	ofs.open("Scores.txt", ios::out | ios::trunc);//opens score file for writing and wipes prior data

	if (lineCount >= 10) {//if there are ten scores stored in the array
		for (int i = 11; i >= 2; i--) {//prints top ten
			ofs << scores.at(i).name << "|" << scores.at(i).win << "%" << scores.at(i).loss << "*" << endl;
		}
	}
	else if (lineCount < 10)//if there are less then 10 scores
	{
		for (int i = lineCount-1; i >= 0; i--)//print all scores
		{
			ofs << scores.at(i).name << "|" << scores.at(i).win << "%" << scores.at(i).loss << "*" << endl;
		}
	}
	ofs.close();//closes file
	//WRITING INTO FILE ENDS--------------------------------------------------------------------------


	return 0;
}
void Board::printHighScore(void)//print high score table onto screen
{
	system("cls");
	cout << "LEADERBOARD" << endl;
	cout << setw(15) << left << "Name" << setw(6) << left << "Wins" << setw(6) << left << "Losses" << endl;
	if (lineCount >= 10) {//if there are ten scores stored in the array
		for (int i = 11; i >= 2; i--) {//prints top ten
			cout << setw(15) << left << scores.at(i).name << setw(6) << left << scores.at(i).win << setw(6) << scores.at(i).loss << endl;
		}
	}
	else if (lineCount < 10)//if there are less then 10 scores
	{
		for (int i = lineCount -1 ; i>= 0; i--)//print all scores
		{
			cout << setw(15) << left << scores.at(i).name << setw(6) << left << scores.at(i).win << setw(6) << scores.at(i).loss << endl;
		}
	}
}