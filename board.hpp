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

    // Rectangular boundary of available moves.
   /* int up;
    int down;
    int left;
    int right;*/

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    void flip_color(vector<int> change);
    bool makeMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    /*int getup();
    int getdown();
    int getleft();
    int getright();
    void setup(int newup);
    void setdown(int newdown);
    void setleft(int newleft);
    void setright(int newright);*/
    void draw();

    void setBoard(char data[]);
    int board_score(Side side);
};

#endif
