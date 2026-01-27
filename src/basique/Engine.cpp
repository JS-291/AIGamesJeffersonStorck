/*
 * Engine.cpp
 *
 *  Created on: Nov 17, 2025
 *      Author: js
 */
#include "Engine.h"

using namespace std;

Engine::Engine(bool l){
	p1Score=0;
	p2Score=0;
	turn=l;
	turnCount=0;
}

void Engine::printBoard(){
	for(Hole h:Engine::board){
		cout << h.n << " R:" << h.r << " B:" << h.b<< " T:" << h.t <<endl;
	}
}

void Engine::fillBoard(){
	for(int i=0;i<16;i++){
		board[i]=Hole{2,2,2,i+1};;
	}
}


void Engine::voidHole(Hole* p){
	int total=0;
	total+=p->b+p->r+p->t;
	p->b=0;
	p->r=0;
	p->t=0;
	if(turn){
		p2Score+=total;
	}else{
		p1Score+=total;
	}
}


void Engine::capture(int n){
    int pos=n;
	Hole* h=&board[((pos%16)+16)%16];
	while(is2or3(h)){
		voidHole(h);
		pos--;
		h=&board[(((pos)%16)+16)%16];
	}
	if(isStarved(false,board)){
		p2Score+=total(board);
		end();
	}else if(isStarved(true,board)){
		p1Score+=total(board);
		end();
	}else if(total(board)<10){
		end();
	}else if(p1Score>=49 or p2Score>=49){
		end();
	}
}

void Engine::red(int p, Hole* h){
    int seeds=h->r;
    int pos=p;
    h->r=0;
    while (seeds > 0) {
        pos=(pos+1)%16;
        if (&board[pos] == h) continue;
        (&board[pos])->r++;
        seeds--;
    }
    capture(pos);
}
void Engine::blue(int p, Hole* h){
    int seeds=h->b;
	if(seeds==0){
		capture((p-1+16)%16);
		return;
	}
    int pos=p;
    h->b=0;
    pos=(pos+1)%16;
    while(seeds>0) {
        (&board[pos])->b++;
        seeds--;
        if(seeds>0)pos=(pos+2)%16;
    }
    capture(pos);
}

void Engine::end(){
	if(p1Score<p2Score){
		cout << "player 2 wins P2 : "<<p2Score<<" P1 : "<<p1Score<<endl;
	}else if(p1Score>p2Score){
		cout << "player 1 wins P1 : "<<p1Score<<" P2 : "<<p2Score<<endl;
	}else{
		cout << "draw P1 : "<<p1Score<<" P2 : "<<p2Score<<endl;
	}
	exit(0);
}

int Engine::tra(int p,bool l, Hole* h){
    int seeds = h->t;
    int pos=p;
    h->t = 0;
    //TR
	if(l){
	    while(seeds>0){
	        pos=(pos+1)%16;
	        if(&board[pos]==h) continue;
	        (&board[pos])->t++;
	        seeds--;
	    }
	//TB
	}else{
	    pos=(pos+1)%16;
	    while(seeds>0) {
	        (&board[pos])->t++;
	        seeds--;
	        pos=(pos+2)%16;
	    }
	}
	return pos;
}

array<Hole,16> Engine::getBoard(){
	array<Hole,16> res;
	for(int i=0;i<16;i++){
		res[i]=board[i];
	}
	return res;
}

int Engine::getP1Score(){
	return p1Score;
}
int Engine::getP2Score(){
	return p2Score;
}

bool Engine::play(int n,string s){
    if ((n % 2 == 0) != turn) {
        cout << "Invalid move" << endl;
        return true;
    }
    int pos=n-1;
    Hole* h = &board[pos];
    //R
    if (s == "R") {
    	if(h->r==0){
    		cout << "Invalid move" << endl;
    		return true;
    	}
    	red(pos,h);
    }
    //B
    else if (s == "B") {
    	if(h->b==0){
    		cout << "Invalid move" << endl;
    		return true;
    	}
    	blue(pos,h);
    }
    //TR
    else if (s == "TR") {
    	if(h->t==0){
    		cout << "Invalid move" << endl;
    		return true;
    	}
    	red(tra(pos,true,h),h);
    }
    //TB
    else if (s == "TB") {
    	if(h->t==0){
    		cout << "Invalid move" << endl;
    		return true;
    	}
    	blue(((tra(pos,false,h)-1)+16)%16,h);
    }
    else {
        cout << "Invalid move" << endl;
        return true;
    }
    turnCount++;
    if(turnCount>=400){
    	end();
    }
    turn = !turn;
    return false;
}



