#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "board.hpp"
#include <string>
#include <unordered_map>

class Player {
private:
    Board * board;
	Side pside;
    Side other;
    unordered_map<string, vector<Move *>> memo;
public:
    int depth;
    Player(Side side);
    Player(Side side, Board * board);
    ~Player();

    Move * minimax1(vector<Move *> move_lst);
    int minimax2(vector<Move *> move_lst, Board * b, int times);
    vector<Move *> get_moves(Board * board, Side side);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

string convert(Board * board, Side side);

#endif
