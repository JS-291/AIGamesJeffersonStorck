/*
 * Engine.h
 *
 *  Created on: Nov 17, 2025
 *      Author: js
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <algorithm>
#include "Utils.h"

class Engine{
	private:
		Hole board[16];
		int p1Score;
		int p2Score;
		int turnCount;
		bool turn;

	public:
		Engine(bool l);
		void printBoard();
		void fillBoard();
		void voidHole(Hole* p);
		void capture(int n);
		void red(int p, Hole* h);
		void blue(int p, Hole* h);
		void end();
		int tra(int p,bool l, Hole*h);
		bool play(int n,std::string s);
		int getP1Score();
		int getP2Score();
		std::array<Hole,16> getBoard();
};

#endif /* ENGINE_H_ */
