#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "dtree.hpp"

class Player {
private:
	//Dtree * dtree;
	Side pside;
    Side other;
public:

	Dtree * dtree;
    Player(Side side);
    Player(Side side, Board * board);
    ~Player();

    void printboardplayer();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
