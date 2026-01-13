//============================================================================
// Name        : Game.cpp
// Author      : JS
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include <string>
#include "Engine.h"
#include "AI.h"

using namespace std;


static void parseMove(const string& s,int& h,string& m) {
    if(isdigit(s[1])){
        h=stoi(s.substr(0,2));
        m=s.substr(2);
    }else{
        h=stoi(s.substr(0,1));
        m=s.substr(1);
    }
}

int main() {
    cin.tie(nullptr);
    Engine engine(false);
    engine.fillBoard();
    AI ai=AI();
    string input;
    while (getline(cin,input)) {
        if (input.rfind("RESULT",0)==0) {
            return 0;
        }
        if(input!="START") {
            int hole;
            string move;
            parseMove(input,hole,move);
            engine.play(hole,move);
        }
        if(engine.isFinished()){
            cout<<engine.resultString()<<'\n';
            cout.flush();
            return 0;
        }
        string AIMove=ai.chooseMove(engine.getBoard(),engine.getP1Score(),engine.getP2Score(),4,engine.getTurn());
        int hole;
        string move;
        parseMove(AIMove,hole,move);
        engine.play(hole,move);
        cout << AIMove << '\n';
        cout.flush();
        if (engine.isFinished()) {
            cout << engine.resultString() << '\n';
            cout.flush();
            return 0;
        }
    }
    return 0;
}

