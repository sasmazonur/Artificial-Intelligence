/*
 * MinimaxPlayer.h
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 *	Edited on: 	May 1, 2020
 *			Edited: Onur Sasmaz
 */

#ifndef MINIMAXPLAYER_H
#define MINIMAXPLAYER_H

#include "OthelloBoard.h"
#include "Player.h"
#include <vector>
using namespace std;
/**
 * This class represents an AI player that uses the Minimax algorithm to play the game
 * intelligently.
 */
class MinimaxPlayer : public Player {
public:

	/**
	 * @param symb This is the symbol for the minimax player's pieces
	 */
	MinimaxPlayer(char symb);

	/**
	 * Destructor
	 */
	virtual ~MinimaxPlayer();

	/**
	 * @param b The board object for the current state of the board
	 * @param col Holds the return value for the column of the move
	 * @param row Holds the return value for the row of the move
	 */
	 	int gameSize = 4;//The 4x4 version of Othello
	 	vector<OthelloBoard*>SuccessorFunc(OthelloBoard b, char symbl);
    void get_move(OthelloBoard* b, int& col, int& row);
		int UtilityFunc(OthelloBoard b);
		int MinValue(OthelloBoard b);
		int MaxValue(OthelloBoard b);

    /**
     * @return A copy of the MinimaxPlayer object
     * This is a virtual copy constructor
     */
    MinimaxPlayer* clone();

private:

};


#endif
