/*
 * Engine.cpp
 *
 *  Created on: Nov 17, 2025
 *      Author: js
 */
#include "Engine.h"
#include <iostream>

using namespace std;

Engine::Engine(bool l) {
    p1Score=0;
    p2Score=0;
    turn=l;
    turnCount=0;
    finished=false;
    limit=false;
}

void Engine::fillBoard() {
    for(int i=0;i<16;i++) {
        board[i]=Hole{2,2,2,i+1};
    }
}

void Engine::voidHole(Hole* p) {
    int total=p->b+p->r+p->t;
    p->b=p->r=p->t=0;
    if(turn)p2Score+=total;
    else p1Score+=total;
}

void Engine::capture(int n) {
    int pos=n;
    Hole* h=&board[(pos+16)%16];

    while(is2or3(h)) {
        voidHole(h);
        pos--;
        h=&board[(pos+16)%16];
    }

    if(isStarved(false,board)){
        p2Score+=total(board);
        finished=true;
    }else if(isStarved(true,board)){
        p1Score+=total(board);
        finished=true;
    }else if(total(board)<10){
        finished=true;
    }else if(p1Score>=49||p2Score>= 49) {
        finished=true;
    }
}

void Engine::red(int p,Hole* h) {
    int seeds=h->r;
    int pos=p;
    h->r=0;
    while(seeds>0) {
        pos=(pos+1)%16;
        if(&board[pos]== h) continue;
        board[pos].r++;
        seeds--;
    }
    capture(pos);
}

void Engine::blue(int p,Hole* h) {
    int seeds=h->b;
    int pos=p;
    h->b=0;

    pos=(pos+1)%16;
    while(seeds>0) {
        board[pos].b++;
        seeds--;
        if (seeds>0) pos=(pos+2)%16;
    }
    capture(pos);
}

int Engine::tra(int p,bool l,Hole* h) {
    int seeds=h->t;
    int pos=p;
    h->t=0;
    if(l){ // TR
        while(seeds > 0){
            pos=(pos+1)%16;
            if(&board[pos]==h) continue;
            board[pos].t++;
            seeds--;
        }
    }else{ // TB
        pos=(pos+1)% 16;
        while(seeds>0) {
            board[pos].t++;
            seeds--;
            if(seeds>0) pos=(pos+2)%16;
        }
    }
    return pos;
}

bool Engine::play(int n,string s) {
    if(finished) return true;
    if((n%2==0)!= turn) return true;
    int pos=n-1;
    Hole* h=&board[pos];
    if(s=="R") {
        if(h->r==0) return true;
        red(pos,h);
    }
    else if(s=="B") {
        if(h->b == 0) return true;
        blue(pos,h);
    }
    else if(s=="TR") {
        if(h->t==0) return true;
        red(tra(pos,true,h),h);
    }
    else if(s == "TB") {
        if(h->t==0) return true;
        blue(((tra(pos,false,h)-1+16)%16),h);
    }
    else{
        return true;
    }
    turnCount++;
    if(turnCount>=400){
      finished=true;
      limit=true;
    }
    turn=!turn;
    return false;
}

array<Hole,16> Engine::getBoard() {
    return board;
}

int Engine::getP1Score(){
    return p1Score;
}

int Engine::getP2Score(){
    return p2Score;
}

bool Engine::getTurn(){
    return turn;
}

bool Engine::isFinished(){
    return finished;
}

string Engine::resultString(){
    if(limit){
        if(p1Score!=p2Score) return "RESULT LIMIT :"+to_string(p1Score)+" "+to_string(p2Score);
        return "RESULT LIMIT DRAW";
    }else{
        if(p1Score!=p2Score) return "RESULT "+to_string(p1Score)+" "+to_string(p2Score);
        return "RESULT DRAW";
    }
}

