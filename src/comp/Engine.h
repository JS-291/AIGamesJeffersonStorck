/*
 * Engine.h
 *
 *  Created on: Nov 17, 2025
 *      Author: js
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <array>
#include <string>
#include <vector>
#include "Utils.h"


class Engine {
private:
    std::array<Hole, 16> board;
    std::string move;
    int p1Score;
    int p2Score;
    bool turn;
    int turnCount;
    bool finished;
    bool limit;
public:
    Engine(bool l);
    void capture(int n);
    void red(int p,Hole* h);
    void blue(int p,Hole* h);
    int tra(int p,bool l,Hole* h);
    void voidHole(Hole* p);
    void fillBoard();
    bool play(int n,std::string s);
    std::array<Hole,16> getBoard();
    int getP1Score();
    int getP2Score();
    bool getTurn();
    bool isFinished();
    std::string resultString();
};

#endif

