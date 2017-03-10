#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side, Board * board) {
    // Will be set to true in test_minimax.cpp.
    dtree = new Dtree(board, nullptr, side);
    pside = side;

    testingMinimax = false;


    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

Player::Player(Side side) {
    dtree = new Dtree(side);
    pside = side;

    testingMinimax = false;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete dtree;
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
    std::cerr << "hello1" << std::endl;
    if (dtree->no_children())
    {
        Side other = (pside == BLACK) ? WHITE : BLACK;
        dtree->SimpleDoMove(opponentsMove, other);
    }
    else
    {
        dtree->DoMove(opponentsMove);
    }

    std::cerr << "After white's move" << std::endl;

    dtree->printboard();
    dtree->init_FindMoves(pside);

    std::cerr << "hello3" << std::endl;
    if (dtree->no_children())
    {
        std::cerr << "serial" << std::endl;
        return NULL;
    }

    std::cerr << "serial2" << std::endl;

    Move * return_move = dtree->chooseMove(pside);

    std::cerr << ((dtree->head)->move_lst).size() << std::endl;

    std::cerr << "after blacks move" << std::endl;

    dtree->printboard();

    std::cerr << "weird" << std::endl;

    return return_move;
}
