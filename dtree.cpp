#include "dtree.hpp"

Dtree::Dtree()
{
	head = new Dnode;
	head->board = new Board;
	head->move = nullptr;
	head->side = UNKNOWN;
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
	if (node->move != nullptr)
		delete node->move;
	delete node;
}

/*
 * Add children to a given node.
 */

void Dtree::FindMoves(Dnode * node)
{
	// No moves available.

	if (!(node->black_avail && node->white_avail))
		return;

	bool valid = false;
	Side s = ((node->side) == BLACK) ? WHITE : BLACK;
	Side other = node->side;

	// Get valid_move rectangle.
/*	int up = (node->board)->getup();
	int down = (node->board)->getdown();
	int left = (node->board)->getleft();
	int right = (node->board)->getright();*/

	Board * newboard = (node->board)->copy();

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Move * newmove = new Move(i, j);
			if (newboard->makeMove(newmove, s))
			{
				Dnode * newDnode = new Dnode(newboard, newmove, s);

				(node->move_lst).push_back(newDnode);

				newboard = (node->board)->copy();
				valid = true;
			}
			else
			{
				delete newmove;
			}

		}
	}

	delete newboard;

	if (valid)
		return;
	
	if (s == BLACK)
		node->black_avail = false;
	else
		node->white_avail = false;

	valid = false;
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Move * newmove = new Move(i, j);
			if (newboard->makeMove(newmove, other))
			{
				Dnode * newDnode = new Dnode(newboard, newmove, s);
				(node->move_lst).push_back(newDnode);
				newboard = (node->board)->copy();
				valid = true;
			}
			else
			{
				delete newmove;
			}
		}
	}

	if (valid)
	return;
	
	if (other == BLACK)
		node->black_avail = false;
	else
		node->white_avail = false;
}

/*
 * Apply find moves to depth 2.
 */

void Dtree::expand()
{
	int depth = calculate_depth();
	if (depth == 0)
	{
		FindMoves(head);
		for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
		{
			std::cerr << ((*i)->move)->getX() << "  " << ((*i)->move)->getY() << std::endl;
			FindMoves(*i);
		}
	}
	else if (depth == 1)
	{
		//std::cerr << "hello3" << std::endl;
		for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
			FindMoves(*i);
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
		if ((x != X) || (y != Y)){
			deleteNode((*i));}
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

int Dtree::calculate_depth()
{
	if ((head->move_lst).empty())
		return 0;

	for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
	{
		if (!((*i)->move_lst).empty())
			return 2; // This is the max depth we allow.
	}

	return 1;
}

void Dtree::printboard()
{
	(head->board)->draw();
}

void Dtree::print_next()
{
	if (head->side == BLACK)
		std::cerr << "\nMOVE LIST BLACK" << std::endl;
	else
		std::cerr << "\nMOVE LIST WHITE" << std::endl;
	
	for (auto i = (head->move_lst).begin(); i != (head->move_lst).end(); i++)
	{
		((*i)->board)->draw();
		std::cerr << std::endl;
	}
	std::cerr << "END LIST\n" << std::endl;
}