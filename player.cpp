#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side, Board * board) {
    // Will be set to true in test_minimax.cpp.
    pside = side;
    other = (pside == BLACK) ? WHITE : BLACK;
    this->board = board;

    testingMinimax = false;


    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

Player::Player(Side side) {
    pside = side;
    other = (pside == BLACK) ? WHITE : BLACK;
    board = new Board;

    testingMinimax = false;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
    for (auto i = memo.begin(); i != memo.end(); i++)
    {
        for (auto j = (i->second).begin(); j != (i->second).end(); j++)
        {
            delete (*j);
        }
    }
}

Move * Player::minimax1(vector<Move *> move_lst)
{
    int num_moves = (int) move_lst.size();

    int * min_score = new int[num_moves];
    int worst_score;

    for (int i = 0; i < num_moves; i++)
    {
        // Save the initial board state and apply a valid move..
        Board * testboard = board->copy();
        testboard->doMove(move_lst[i], pside);

        // List of opponent's moves.
        vector<Move*> op_moves_lst = testboard->valid_moves(other);
        min_score[i] = 100000000;

        if (!(op_moves_lst.empty()))
        {
            Board * boardsave = testboard->copy();
            for (int j = 0; j < (int) op_moves_lst.size(); j++)
            {
                delete testboard;
                testboard = boardsave->copy();

                // Apply opponent's move.
                testboard->doMove(op_moves_lst[j], other);

                // Now check player's moves.
                vector<Move *> p_moves = testboard->valid_moves(pside);
                if (!(p_moves.empty()))
                {
                    worst_score = minimax2(p_moves, testboard, depth);
                }
                else
                {
                    worst_score = testboard->board_score(pside);
                }

                if (worst_score < min_score[i])
                    min_score[i] = worst_score;
            }
            delete boardsave;
        }
        else
            min_score[i] = testboard->board_score(pside);

        delete testboard;
    }

    int max_min = -1000000;
    int ind = -1;
    for (int i = 0; i < num_moves; i++)
    {
        if (min_score[i] > max_min)
        {
            max_min = min_score[i];
            ind = i;
        }
    }

    delete[] min_score;
    return move_lst[ind];
}

int Player::minimax2(vector<Move *> move_lst, Board * b, int times)
{
    int num_moves = (int) move_lst.size();

    int * min_score = new int[num_moves];
    int worst_score;

    for (int i = 0; i < num_moves; i++)
    {
        Board * testboard = b->copy();
        testboard->doMove(move_lst[i], pside);
        //vector<Move *> op_moves_lst = testboard->valid_moves(other);
        vector<Move *> op_moves_lst = get_moves(testboard, other);
        min_score[i] = 100000000;

        if (!(op_moves_lst.empty()))
        {
            Board * boardsave = testboard->copy();
            for (int j = 0; j < (int) op_moves_lst.size(); j++)
            {
                delete testboard;
                testboard = boardsave->copy();
                testboard->doMove(op_moves_lst[j], other);

                if (times < 2)
                {
                    //vector<Move *> p_lst = testboard->valid_moves(pside);
                    vector<Move *> p_lst = get_moves(testboard, pside);
                    if (!(p_lst.empty()))
                        worst_score = minimax2(p_lst, testboard, times + 1);
                    else
                        worst_score = testboard->board_score(pside);
                }
                else
                    worst_score = testboard->board_score(pside);
            }
            delete boardsave;
            if (worst_score < min_score[i])
                min_score[i] = worst_score;
        }
        else
            min_score[i] = testboard->board_score(pside);
        delete testboard;
    }

    int ret = 10000000;
    for (int i = 0; i < num_moves; i++)
    {
        if (min_score[i] < ret)
            ret = min_score[i];
    }

    delete[] min_score;
    return ret;
}


/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    board->doMove(opponentsMove, other);

    if ((board->gettaken()).count() > 58)
        depth = 0;
    else
        depth = 1;

    vector<Move *> move_lst = get_moves(board, pside);

    if (!(move_lst.empty()))
    {
        Move * best_move = minimax1(move_lst);
        board->doMove(best_move, pside);
        return best_move;
    }

    return nullptr;
}

vector<Move *> Player::get_moves(Board * board, Side side)
{
    string key = convert(board, side);

    unordered_map<string, vector<Move *>>::const_iterator got;
    got = memo.find(key);

    if (got == memo.end()) {
        vector<Move *> to_add = board->valid_moves(side);
        pair<string, vector<Move *>> newpair(key, to_add);
        memo.insert(newpair);
        return to_add;
    }
    else
    {
        return got->second;
    }
}

string convert(Board * board, Side side)
{
    string ret_string;
    string black_string;
    for (int i = 0; i < 64; i++)
    {
        if ((board->gettaken())[i] == 0) ret_string.push_back('0');
        else ret_string.push_back('1');

        if((board->getblack())[i] == 0) black_string.push_back('0');
        else black_string.push_back('1');
    }
    ret_string.append(black_string);

    if (side == BLACK) ret_string.push_back('0');
    else ret_string.push_back('1');

    return ret_string;
}
