/*
 * AI.cpp
 *
 *  Created on: Dec 1, 2025
 *      Author: js
 */

#include "AI.h"

using namespace std;

AI::AI(bool p){
	player=p;
	srand((unsigned)time(NULL));
}


int AI::voidHole(Hole* p){
	int total=p->r+p->b+p->t;
	p->r=0;
	p->b=0;
	p->t=0;
	return total;
}

int AI::capture(array<Hole,16>& b,int n){
	int gained=0;
	int pos=n;
	Hole* h=&b[(pos%16+16)%16];
	while(is2or3(h)){
		gained+=voidHole(h);
		pos=(pos-1+16)%16;
		h=&b[(pos%16+16)%16];
	}
	return gained;
}


int AI::redMove(array<Hole,16>& b,int p){
	Hole* h=&b[p];
	int seeds=h->r;
	h->r=0;
	int pos=p;
	while(seeds>0){
		pos=(pos+1)%16;
		if(&b[pos]==h) continue;
		b[pos].r++;
		seeds--;
	}
	return capture(b,pos);
}

int AI::blueMove(array<Hole,16>& b, int p){
	Hole* h=&b[p];
	int seeds=h->b;
	h->b=0;
	int pos=(p+1)%16;
	while(seeds>0){
		b[pos%16].b++;
		seeds--;
		if(seeds>0)pos=(pos+2)%16;
	}
	return capture(b,pos);
}

int AI::blueMove(array<Hole,16>& b, Hole* h, int p){
	int seeds=h->b;
	if(seeds==0){
		return capture(b,(p-1+16)%16);
	}
	h->b=0;
	int pos=(p+1)%16;
	while(seeds>0){
		b[pos%16].b++;
		seeds--;
		if(seeds>0)pos=(pos+2)%16;
	}
	return capture(b,pos);
}

int AI::transparentMove(array<Hole,16>& b,int p,bool redStyle){
	Hole* h=&b[p];
	int seeds=h->t;
	h->t=0;
	int pos=p;
	if(redStyle){
		while(seeds>0){
			pos=(pos+1)%16;
			if(&b[pos]==h) continue;
			b[pos].t++;
			seeds--;
		}
	}else{
		pos=(pos+1)%16;
		while(seeds>0){
			b[pos%16].t++;
			seeds--;
			pos=(pos+2)%16;
		}
	}
	return pos;
}

Node* AI::createNode(Node* p,array<Hole,16> a,int moveType,int hole,bool turn){
	Node* res = new Node;
	res->p1Score=p->p1Score;
	res->p2Score=p->p2Score;
	array<Hole,16> board=a;
	int gained=0;
	int pos;
	switch(moveType){
		case 1:
			gained=redMove(board,hole);
			res->name=to_string(hole+1)+"R";
			break;

		case 2:
			gained=blueMove(board,hole);
			res->name=to_string(hole+1)+"B";
			break;

		case 3:
			pos=transparentMove(board,hole,true);
			gained=redMove(board,pos);
			res->name=to_string(hole+1)+"TR";
			break;

		case 4:
			pos=transparentMove(board,hole,false);
			gained=blueMove(board,&board[pos],(pos-1+16)%16);
			res->name=to_string(hole+1)+"TB";
			break;
	}
	if(!turn){
		res->p1Score += gained;
		if (isStarved(turn,board)){
			res->p1Score+=total(board);
			res->endState=true;
		}
	}
	else{
		res->p2Score += gained;
		if (isStarved(turn,board)){
			res->p2Score+=total(board);
			res->endState=true;
		}
	}
	res->board=board;
	res->prev=p;
	res->endState=res->endState||checkEndState(res);
	return res;
}

vector<Node*> AI::findMove(Node* n, array<Hole,16> a, bool turn){
	vector<Node*> res;
	int i=turn ? 1 : 0;
	while(i<16){
		Hole temp=a[i];
		//R
		if(temp.r>0)
			res.push_back(createNode(n,a,1,i,turn));
		//B
		if(temp.b>0)
			res.push_back(createNode(n,a,2,i,turn));
		//TR TB
		if(temp.t>0){
			res.push_back(createNode(n,a,3,i,turn));
			res.push_back(createNode(n,a,4,i,turn));
		}
		i+=2;
	}
	return res;
}

int AI::minimax(Node* n,int d,int alpha,int beta,bool turn,bool m) {
    if (d==0||n->endState) {
        return eval(n,!turn);
    }
    if(m){
        int best=numeric_limits<int>::min();
        n->next=findMove(n,n->board,turn);
        for (Node* node:n->next) {
            int val=minimax(node,d-1,alpha,beta,!turn,!m);
            best=max(best,val);
            alpha=max(alpha,best);
            if(beta<=alpha) break;
        }
        return best;
    }else{
        int best=numeric_limits<int>::max();
        n->next=findMove(n,n->board,turn);
        for (Node* node:n->next) {
            int val=minimax(node,d-1,alpha,beta,!turn,false);
            best=min(best,val);
            beta=min(beta,best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

string AI::chooseMove(array<Hole,16> b,int p1,int p2,int d,bool t){
	Node* node=new Node;
	vector<Node*> moves;
	string res;
	int alpha=numeric_limits<int>::min();
	node->board=b;
	node->p1Score=p1;
	node->p2Score=p2;
	node->endState=checkEndState(node);
	node->next=findMove(node,node->board,t);
	for(Node* no:node->next){
		int val=minimax(no,d-1,alpha,numeric_limits<int>::max(),!t,false);
		if(val>alpha){
			alpha=val;
			res=no->name;
	        moves.clear();
	        moves.push_back(no);
		}else if(val==alpha){
			moves.push_back(no);
		}
	}
	res=moves[rand()%moves.size()]->name;
	freeTree(node);
	return res;
}

int AI::eval(Node* n,bool t){
	int res=0;
	if(t){
		res+=(n->p2Score-n->p1Score)*20;

	    if(isStarved(true,n->board)) res-=50;
	    if(isStarved(false,n->board)) res+=50;

		if(n->endState){
			if(n->p2Score>n->p1Score){
				res+=500;
			}else if(n->p2Score<n->p1Score){
				res-=500;
			}else{
				res+=10;
			}
		}

		for(int i=0;i<16;i=i+2){
			if(isEmpty(&n->board[i])) res+=2;
			else{
				if(n->board[i].r>0){
					if(n->board[i].t>0) res-=2*captureChain(i,(n->board[i].r+n->board[i].t),n->board,true);
					else res-=2*captureChain(i,(n->board[i].r),n->board,true);
				}
				if(n->board[i].b>0){
					if(n->board[i].t>0) res-=2*captureChain(i,(n->board[i].b+n->board[i].t),n->board,false);
					else res-=2*captureChain(i,(n->board[i].b),n->board,false);
				}
				res-=n->board[i].t*2;
				res-=n->board[i].r*1;
				if(is1or2(&n->board[i])) res+=3;
			}
		}
		for(int i=1;i<16;i=i+2){
			if(is1or2(&n->board[i])) res-=3;
			if(n->board[i].r>0){
				if(n->board[i].t>0) res+=2*captureChain(i,(n->board[i].r+n->board[i].t),n->board,true);
				else res+=2*captureChain(i,(n->board[i].r),n->board,true);
			}
			if(n->board[i].b>0){
				if(n->board[i].t>0) res+=2*captureChain(i,(n->board[i].b+n->board[i].t),n->board,false);
				else res+=2*captureChain(i,(n->board[i].b),n->board,false);
			}
		}
	}else{
		res+=(n->p1Score-n->p2Score)*20;

	    if(isStarved(false, n->board)) res -= 50;
	    if(isStarved(true, n->board)) res += 50;

		if(n->endState){
			if(n->p2Score>n->p1Score){
				res-=500;
			}else if(n->p2Score<n->p1Score){
				res+=500;
			}else{
				res+=30;
			}
		}

		for(int i=1;i<16;i=i+2){
			if(isEmpty(&n->board[i])) res+=2;
			else{
				if(n->board[i].r>0){
					if(n->board[i].t>0) res-=2*captureChain(i,(n->board[i].r+n->board[i].t),n->board,true);
					else res-=2*captureChain(i,(n->board[i].r),n->board,true);
				}
				if(n->board[i].b>0){
					if(n->board[i].t>0) res-=2*captureChain(i,(n->board[i].b+n->board[i].t),n->board,false);
					else res-=2*captureChain(i,(n->board[i].b),n->board,false);
				}
				res-=n->board[i].t*2;
				res-=n->board[i].r*1;
				if(is1or2(&n->board[i])) res+=3;
			}

		}
		for(int i=0;i<16;i=i+2){
			if(is1or2(&n->board[i])) res-=3;
			if(n->board[i].r>0){
				if(n->board[i].t>0) res+=2*captureChain(i,(n->board[i].r+n->board[i].t),n->board,true);
					else res+=2*captureChain(i,(n->board[i].r),n->board,true);
			}
			if(n->board[i].b>0){
				if(n->board[i].t>0) res+=2*captureChain(i,(n->board[i].b+n->board[i].t),n->board,false);
				else res+=2*captureChain(i,(n->board[i].b),n->board,false);
			}
		}
	}
	return res;
}

int AI::captureChain(int i,int s,array<Hole,16>& b,bool c){
	int res=0;
	int pos;
    int st=0;
	if(c){
		pos=(i+s)%16;
        while(is1or2(&b[pos])&&st< 16){
            res++;
            pos=(pos-1+16)%16;
            st++;
        }
	}else{
		pos=(i+s*2)%16;
        while(is1or2(&b[pos])&&st<16){
            res++;
            pos=(pos-2+16)%16;
            st++;
        }
	}
	return res;
}
bool AI::checkEndState(Node* n){
	if(total(n->board)<10){
		return true;
	}else if(n->p1Score>=49 or n->p2Score>=49){
		return true;
	}
	return false;
}

void AI::freeTree(Node* n) {
    if(!n) return;
    for(Node* node:n->next) {
        freeTree(node);
    }
    delete n;
}

bool AI::getPlayer(){
	return player;
}


