/*
 * Utils.h
 *
 *  Created on: Dec 2, 2025
 *      Author: js
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <vector>
#include <array>

struct Hole {
    int r;
    int b;
    int t;
    int n;
};

struct Node {
	std::string name;
	Node* prev;
	std::vector<Node*> next;
	std::array<Hole,16> board;
	int p1Score;
	int p2Score;
	bool endState;
};

bool isEmpty(Hole* p);
bool is2or3(Hole* p);
bool is1or2(Hole* p);
bool isStarved(bool l, Hole b[16]);
bool isStarved(bool l, std::array<Hole,16> b);
int total(Hole b[16]);
int total(std::array<Hole,16> b);




#endif /* UTILS_H_ */
