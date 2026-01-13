//============================================================================
// Name        : Game.cpp
// Author      : JS
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>
#include <regex>
#include <algorithm>
#include "Engine.h"
#include "AI.h"
using namespace std;


int main() {
	bool first,second,third,fourth,ai,turn=false;
	bool player;
	auto re=regex(R"(^((1[0-6])|([1-9]))(TB|TR|R|B)$)");
	auto re2=regex(R"(^([1-9])(TB|TR|R|B)$)");
	string input;
	Engine E=Engine(false);
	E.fillBoard();
	do{
		cout << "Single or AI :" << endl;
		cin >> input;
		if(input == "Single"){
			ai=false;
		}else if(input == "AI"){
			ai=true;
		}else{
			cout << "Invalid input"<< endl;
			fourth=true;
			continue;
		}
		fourth=false;
	}while(fourth);
	cout << "*****************************"<< endl;
	if(ai){
		do{
			cout << "P1 or P2 :" << endl;
			cin >> input;
			if(input == "P1"){
				player=false;
			}else if(input == "P2"){
				player=true;
			}else{
				cout << "Invalid input"<< endl;
				first=true;
				continue;
			}
			first=false;
		}while(first);
		AI A=AI(!player);
		E.printBoard();
		while(true){
			cout << "*****************************"<< endl;
			if(turn){
				cout << "P2 moves :"<< endl;
			}else{
				cout << "P1 moves :"<<endl;
			}
			if(turn!=player){
				string move=A.chooseMove(E.getBoard(),E.getP1Score(),E.getP2Score(),4,turn);
				cout<<move<<endl;
				if (regex_match(move,re2)){
					E.play(stoi(move.substr(0, 1)),move.substr(1));
				}else{
					E.play(stoi(move.substr(0, 2)),move.substr(2));
				}
			}else{
				do{
					cin >> input;
					if(regex_match(input,re)){
						if (regex_match(input,re2)){
							third=E.play(stoi(input.substr(0, 1)),input.substr(1));
						}else{
							third=E.play(stoi(input.substr(0, 2)),input.substr(2));
						}
					}else{
						cout << "Invalid move"<< endl;
						second=true;
						continue;
					}
					second=false;
				}while(second or third);
			}
			cout << "*****************************"<< endl;
			turn=!turn;
			E.printBoard();
		}
	}else{
		E.printBoard();
		while(true){
			cout << "*****************************"<< endl;
			do{
				if(turn){
					cout << "P2 moves :"<< endl;
				}else{
					cout << "P1 moves :"<<endl;
				}
				cin >> input;
				if(regex_match(input,re)){
					if (regex_match(input,re2)){
						third=E.play(stoi(input.substr(0, 1)), input.substr(1));
					}else{
						third=E.play(stoi(input.substr(0, 2)), input.substr(2));
					}
				}else{
					cout << "Invalid move"<< endl;
					second=true;
					continue;
				}
				second=false;
			}while(second or third);
			turn=!turn;
			cout << "*****************************"<< endl;
			E.printBoard();
		}
	}
	return 0;
}
