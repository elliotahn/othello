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
    other = (side == BLACK) ? WHITE : BLACK;

    testingMinimax = false;


    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

Player::Player(Side side) {
    dtree = new Dtree();
    pside = side;
    other = (side == BLACK) ? WHITE : BLACK;

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
    Move * opmove;
    std::cerr << "\n----NEW ROUND-----" << std::endl;

    if (opponentsMove == nullptr) {
        std::cerr << "white: null" << std::endl;
        opmove = nullptr;
    }
    else {
        int X = opponentsMove->getX();
        int Y = opponentsMove->getY();
        opmove = new Move(X, Y);
        std::cerr << "white:\t" << X << "   " << Y << std::endl;
    }

    std::cerr << dtree->calculate_depth() << std::endl;

    if (dtree->calculate_depth() == 0)
    {
        if ((dtree->head)->side == UNKNOWN)
        {
            dtree->SimpleDoMove(opmove, other);
            (dtree->head)->side = other;
        }
    }
    else
    {
        dtree->DoMove(opmove);
    }

    // std::cerr << dtree->calculate_depth() << std::endl;

    // std::cerr << "After White's move" << std::endl;

    dtree->printboard();

    dtree->expand();



    // dtree->print_next();

    // std::cerr << dtree->calculate_depth() << std::endl;

    if (dtree->calculate_depth() == 0)
        return nullptr;

    Move * return_move = dtree->chooseMove(pside);

    // if (return_move == nullptr)
    //     std::cerr << "black: null" << std::endl;
    // else {
    //     int X1 = return_move->getX();
    //     int Y1 = return_move->getY();
    //     std::cerr << "black:\t" << X1 << "   " << Y1 << std::endl;
    // }

    // std::cerr << dtree->calculate_depth() << std::endl;

    // std::cerr << "After Black's move" << std::endl;

    // dtree->printboard();

    // std::cerr << return_move << std::endl;
    return return_move;
}
