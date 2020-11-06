/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 *	Edited on: 	May 1, 2020
 *			Edited: Onur Sasmaz
 */
#include <iostream>
#include <assert.h>
#include <algorithm>    // std::max/min
#include "MinimaxPlayer.h"

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

// Utility function that determines the "goodness" of a terminal state.
int MinimaxPlayer::UtilityFunc(OthelloBoard b){
    return (b.count_score(b.get_p1_symbol()) - b.count_score(b.get_p2_symbol()));
}

//	This function takes the current state of the game and generates all
//the successors that can be reached within one move of the current state.
vector<OthelloBoard*>MinimaxPlayer::SuccessorFunc(OthelloBoard b, char symbl){
	vector<OthelloBoard*> SuccessorVec;
	//gamesize assumed 4x4, Can be changed inside MinimaxPlayer.h
	//iterate thrue cols and rows
	for(int i=0; i<gameSize; i++){
		for(int j=0; j<gameSize; j++){
			//Check if its a legal move and Cell is empty
			if(b.is_cell_empty(j,i) && b.is_legal_move(j,i,symbl)){
				//Make a copy of the Board
				OthelloBoard* State = new OthelloBoard(b);
				State->play_move(j,i,symbl);
				State->setR(i);
				State->setC(j);
				SuccessorVec.push_back(State);
			}
		}
	}

	return SuccessorVec;
}

// Min-Value function
//Function MIN-VALUE(state) returns a utility value
//    if TERMINAL-TEST(state) then return UTILITy(state)
//    v <- inf
//    for each a in ACTIONS(state) do
//        v<-MIN(v, MAX-VALUE(RESULT(s,a)))
//    return v
int MinimaxPlayer::MinValue(OthelloBoard b){
	//bool has_legal_moves_remaining(char symbol): if false, the game ends.
	//char get_p2_symbol(): gets the symbol for player 2

	//Check if the Game is Over
	if(!(b.has_legal_moves_remaining(b.get_p2_symbol()))
	&& !(b.has_legal_moves_remaining(b.get_p1_symbol()))){
		return UtilityFunc(b);
	}
	//Get max value from algorithm library
	int v = numeric_limits<int>::max();
	vector<OthelloBoard*> SuccessorVec = SuccessorFunc(b, b.get_p2_symbol());
	for(int i = 0; i < SuccessorVec.size(); i++){
		v = min(v,MaxValue(*SuccessorVec[i]));
	}
	return v;
}

//Max-Value
//Function MAX-VALUE(state) returns a utility value
//    if TERMINAL-TEST(state) then return UTILITy(state)
//    v <- -inf
//    for each a in ACTIONS(state) do
//        v<-MAX(v, MIN-VALUE(RESULT(s,a)))
//    return v
int MinimaxPlayer::MaxValue(OthelloBoard b){
	//bool has_legal_moves_remaining(char symbol): if false, the game ends.
	//char get_p2_symbol(): gets the symbol for player 2

	//Check if the Game is Over
	if(!(b.has_legal_moves_remaining(b.get_p2_symbol()))
	&& !(b.has_legal_moves_remaining(b.get_p1_symbol()))){
		return UtilityFunc(b);
	}
	//Get min value from algorithm library
	int v = numeric_limits<int>::min();
	vector<OthelloBoard*> SuccessorVec = SuccessorFunc(b, b.get_p1_symbol());
	for(int i = 0; i < SuccessorVec.size(); i++){
		v = max(v,MinValue(*SuccessorVec[i]));
	}
	return v;
}

//MINIMAX-DESCISION
//Function MINIMAX-DESCISION(state) returns an action
//    return argmax_a € ACTIONS_(s) MIN-VALUE(RESULT(state,a))
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	// OthelloBoard - int col = -1; int row = -1;
		int temp_row = -1, temp_col = -1, maxVal;

		//The player that moves first is assumed to be the maximizing player
		//Get max value from algorithm library
		int vMin = numeric_limits<int>::max();
		vector<OthelloBoard*> SuccessorVec = SuccessorFunc(*b, get_symbol());

		//The player that moves first is assumed to be the maximizing player
		for(int i = 0; i < SuccessorVec.size(); i++){
			maxVal = MaxValue(*SuccessorVec[i]);
			if(maxVal < vMin){
				vMin = maxVal;
				//Get the coresponding rows and cols
				temp_row = SuccessorVec[i]->getR();
				temp_col = SuccessorVec[i]->getC();
			}
		}
		row = temp_row;
		col = temp_col;
}

//Player Clone
MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
