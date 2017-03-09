#include "dtree.hpp"

Dtree::Dtree(Side s)
{
	head = new Dnode;
	head->board = new Board;
	head->move = nullptr;
	head->side = s;
}

Dtree::Dtree(Board * b, Move * m, Side s)
{
	head = new Dnode(b, m, s);
}

Dtree::~Dtree()
{
	deleteNode(head);
}

/*
 * Delete a node on the tree and all its children.
 */

void Dtree::deleteNode(Dnode * node)
{
	if (!((node->move_lst).empty()))
	{
		for (auto i = (node->move_lst).begin(); i != (node->move_lst).end(); i++)
		{
			deleteNode(*i);
		}
	}

	delete node->board;
	delete node->move;
	delete node;
}

/*
 * Add children to a given node.
 */

void Dtree::FindMoves(Dnode * node, Side s)
{
	// No moves available.
	if (!(node->black_avail && node->white_avail))
		return;

	// Get valid_move rectangle.
	int up = (node->board)->getup();
	int down = (node->board)->getdown();
	int left = (node->board)->getleft();
	int right = (node->board)->getright();

	Board * newboard = (node->board)->copy();

	for (int i = left; i <= right; i++)
	{
		for (int j = up; j <= down; j++)
		{
			Move * newmove = new Move(i, j);
			if (newboard->makeMove(newmove, s))
			{
				Dnode * newDnode = new Dnode(newboard, newmove, s);
				(node->move_lst).push_back(newDnode);
				newboard = (node->board)->copy();
			}
			else
			{
				delete newmove;
			}
		}
	}

	if ((node->move_lst).empty())
	{
		if (s == BLACK)
			node->black_avail = false;
		else
			node->white_avail = false;
	}
	delete newboard;
}

/*
 * Apply find moves to depth 2.
 */

void Dtree::init_FindMoves(Side s)
{
	Side other = (s == BLACK) ? WHITE : BLACK;
	FindMoves(head, s);
	for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
	{
		FindMoves(*i, other);
	}
}

/*
 * Choose a move from the node such that the opponent's score is minimized.
 * We already assume that move_lst for the head node was already calculated.
 * After the optimal move is chosen, the function deletes all the irrelevent
 * nodes and the chosen move becomes the head node in the Dtree class.
 */

Move * Dtree::chooseMove(Side s)
{
	Move * best_move;
	int best_worst_score = -10000;
	int move_score;

	// Find the best move.
	for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
	{
		move_score = getWorstScore(*i, s);
		if (move_score > best_worst_score)
		{
			best_worst_score = move_score;
			best_move = (*i)->move;
		}
	}

	DoMove(best_move);

	return best_move;
}

void Dtree::SimpleDoMove(Move *m, Side s)
{
	(head->board)->doMove(m, s);
}

void Dtree::DoMove(Move * m)
{
	if (m == nullptr)
		return;

	int x = m->getX();
	int y = m->getY();
	Dnode * newhead;

	// Delete all nodes that are not m or its children.

	for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
	{
		int X = ((*i)->move)->getX();
		int Y = ((*i)->move)->getY();
		if ((x != X) || (y != Y))
			deleteNode((*i));
		else
			newhead = *i;
	}

	delete head->board;
	delete head->move;
	delete head;
	head = newhead;

}

int Dtree::getWorstScore(Dnode * node, Side s)
{
	if ((node->move_lst).empty())
	{
		return (node->board)->board_score(s);
	}
	
	int worst_score = 10000;
	int move_score;

	for (auto i = (node->move_lst).begin(); i != (node->move_lst).end(); i++)
	{
		move_score = getWorstScore(*i, s);
		if (move_score < worst_score)
			worst_score = move_score;
	}

	return worst_score;
}

void Dtree::printboard()
{
	(head->board)->draw();
}