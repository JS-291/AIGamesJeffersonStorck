/*
 * AI.h
 *
 *  Created on: Dec 1, 2025
 *      Author: js
 */

#ifndef AI_H_
#define AI_H_

#include <vector>
#include <array>
#include <limits>
#include <cstdlib>
#include "Utils.h"

class AI {
	private:
		bool player;
	public:
		AI(bool p);
		int minimax(Node* n,int d,int alpha,int beta,bool t,bool max);
		void freeTree(Node*n);
		std::string chooseMove(std::array<Hole,16> b,int p1,int p2,int d,bool t);
		int voidHole(Hole* p);
		int captureChain(int i,int s,std::array<Hole,16>& b, bool c);
		int capture(std::array<Hole,16>& b, int n);
		int redMove(std::array<Hole,16>& b, int p);
		int blueMove(std::array<Hole,16>& b, int p);
		int eval(Node* n, bool t);
		int transparentMove(std::array<Hole,16>& b, int p, bool redStyle);
		bool checkEndState(Node* n);
		bool getPlayer();
		Node* createNode(Node* p,std::array<Hole,16> a,int moveType,int hole,bool turn);
		std::vector<Node*> findMove(Node* n,std::array<Hole,16> a,bool turn);
};

#endif /* AI_H_ */
