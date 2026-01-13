/*
 * Utils.cpp
 *
 *  Created on: Dec 2, 2025
 *      Author: js
 */

#include "Utils.h"

using namespace std;

bool isEmpty(Hole* p){
	return p->b==0 and p->r==0 and p->t==0;
}

bool is2or3(Hole* p){
	return p->r+p->t+p->b==2 or p->r+p->t+p->b==3;
}

bool is1or2(Hole* p){
	return p->r+p->t+p->b==1 or p->r+p->t+p->b==2;
}

bool isStarved(bool l, Hole b[16]){
	int i;
	if(l){
		i=1;
		while(i<16){
			if(!isEmpty(&b[i])){
				return false;
			}
			i=i+2;
		}
	}else{
		i=0;
		while(i<15){
			if(!isEmpty(&b[i])){
				return false;
			}
			i=i+2;
		}
	}
	return true;
}

bool isStarved(bool l, array<Hole,16> b){
	int i;
	if(l){
		i=1;
		while(i<16){
			if(!isEmpty(&b[i])){
				return false;
			}
			i=i+2;
		}
	}else{
		i=0;
		while(i<15){
			if(!isEmpty(&b[i])){
				return false;
			}
			i=i+2;
		}
	}
	return true;
}

int total(Hole b[16]){
	int res=0;
    for (int i=0;i<16;i++) {
        res+=b[i].r;
        res+=b[i].b;
        res+=b[i].t;
    }
	return res;
}

int total(array<Hole,16> b){
	int res=0;
    for (int i=0;i<16;i++) {
        res+=b[i].r;
        res+=b[i].b;
        res+=b[i].t;
    }
	return res;
}


