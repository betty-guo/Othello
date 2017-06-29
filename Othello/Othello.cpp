// Othello.cpp : Defines the entry point for the console application.
//Betty Guo
//ICS 3UI Summative
//17/06/15
//Mr Damian

#include "stdafx.h"
#include <iostream>
#include "othello1.h"
#include "windows.h"
#include <string>
using namespace std;

int main()
{
	//INSTRUCTIONS__________________________________
	cout << "OTHELLO" << endl;
	cout << "Instructions: " << endl;
	cout << "1. The purpose of the game is to outflank your opponents pieces \nand have the most pieces at the end of the game (when neither player can move)\n In order to outflank an opponent, their pieces must be bookended by two of your pieces\n If that occurs, the pieces will then flip to your color" << endl;//fix
	cout << "2. In order to place a piece type the row followed by the column e.g. (4g)." << endl;
	cout << "NOTE: column letter is not case sensitive" << endl;
	cout << "3. In order to pass, enter 0 (only possible if no valid moves).  \nIf on your turn you cannot outflank and flip at least one opposing disc, your must pass and your opponent moves again. However, if a move is available to you, you may not forfeit your turn." << endl;
	cout << "4. In order to quit the game type gg and press enter." << endl;
	cout << "5. BLACK always goes first. (in 1 player move, you are black)" << endl;
	//END OF INSTRUCTIONS___________________________
	int md;//mode
	int w;//level
	string m;//player input for move

	cout << "Enter mode (1 or 2 players): " << endl;//get mode
	cin >> md;

	Board brd;//create board

	brd.sent = false; //errorproofing
	brd.mode = md; // initialize mode

	if (md == 1) {//if one player
		cout << "Enter level (1-3): " ;//get level
		cin >> w;
		brd.level = w;//initialize level
	}
	if (md ==2) //if two players
	{
		getline(cin, m);//resets get line (look into)

		cout << "Input player one name (white): ";//gets player one name
		getline(cin, brd.player1.name);
		cout << endl;

		cout << "Input player two name (black): ";//gets player two name
		getline(cin, brd.player2.name);
		cout << endl;
	}

	brd.reset();//initialize board

	if (brd.mode == 1) {// 1 player
		while (!brd.go) {//while not gameover
			try {
				cout << "Your move: ";
				cin >> m;
				if (m == "gg")//if input quit
				{
					brd.go = true;//if gameover
					cout << "End." << endl;//output end notice

					break;
				}
				brd.playerMove(stringParse(m), 2);//take user input and switch into playerMove input (int form)
			}
			catch (nonumber problem) { cout << problem.what() << endl; }//output error message
		}
	}
	
	int c = 1; //player/color of piece marker

	if (brd.mode == 2) {//two player
		while (!brd.go) {//while not gameover
				brd.PlayerCanMove = true;//allows printing on screen 

				if (!brd.sent)//if no error thrown
				{
					c = 3 - c;//change player
				}
			try{ 
				brd.sent = false;//reset error thrown sentinel
				if (c==1)//if white
				{
					cout << brd.player1.name << " turn" << endl;
				}
				else//if black
				{
					cout << brd.player2.name << " turn" << endl; 
				}

				m.clear();//clear player input variable
				cin >> m;//get player input

				if (m == "gg")//if quit
				{
					brd.go = true;//gamover = true
					cout << "End." << endl;

					break;
				}
				brd.playerMove(stringParse(m), c);//take user input and switch into playerMove input
			}
			catch (nonumber problem)
			{
				cout << problem.what() << endl;//outputs error message
				brd.sent = true;//sets error sentinel to true = allows faulty player another input
			}
		}
		brd.readHighScore();//calls highscore function in 2 player
	}
	
	system("pause");//when gameover leave info on screen until user presses key
	return 0;

}//current error: clear page, make animation/ computer move more seamless, skip has issues
//do: prompts, highscore
//issues: can't open file, no gameovber sentinel, valid move has issues sometimes
