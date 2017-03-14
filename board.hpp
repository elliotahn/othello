#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <bitset>
#include "common.hpp"
#include <vector>
#include <iostream>
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();
    void draw();
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    void setBoard(char data[]);

    int board_score(Side side);

    bitset<64> getblack() {return black;}
    bitset<64> gettaken() {return taken;}

    vector<Move *> valid_moves(Side side);
};

#endif
