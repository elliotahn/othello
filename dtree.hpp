/*
 * dtree class
 * Each node contains a node called Dnode.
 * Each Dnode contains a pointer to a board, the last move which got
 * us to that board, the side which represents whose turn it is to make a move
 * on the given board, and a vector of legal moves for that player.
 */

#ifndef __DTREE_HPP__
#define __DTREE_HPP__

#include "board.hpp"

struct Dnode {
	Board * board;
	Move * move;
	Side side;					// Player who made the last move.
	bool black_avail;
	bool white_avail;
	vector<Dnode *> move_lst;

	Dnode()
	{
		black_avail = true;
		white_avail = true;
		this->side = UNKNOWN;
	}

	Dnode(Board * b, Move * m, Side s)
	{
		this->board = b;
		this->move = m;
		this->side = s;
		black_avail = true;
		white_avail = true;
	}
};

class Dtree {
private:
	//Dnode * head;
public:
	Dnode * head;
	Dtree();
	Dtree(Board * b, Move * m, Side s);
	~Dtree();
	bool no_children() {return (head->move_lst).empty();}
	void deleteNode(Dnode * node);
	void FindMoves(Dnode * node);
	void expand();
	Move * chooseMove(Side s);
	void SimpleDoMove(Move * m, Side s);
	void DoMove(Move * m);
	int getWorstScore(Dnode * mode, Side s);
	int calculate_depth();
	void printboard();
	void print_next();
};

#endif
