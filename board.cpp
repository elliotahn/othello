#include "board.hpp"

// Global arrays which represent key locations on the board.
int corners[4] = {0, 7, 56, 63};
int diag1[4] = {9, 14, 49, 54};
int diag2[4] = {18, 21, 42, 45};
int edge1[4][2] = {{1, 8}, {6, 15}, {48, 57}, {55, 62}};
int edge2[4][2] = {{2, 16}, {5, 23}, {40, 58}, {61, 47}};
int otheredges[8] = {3, 4, 24, 32, 31, 39, 59, 60};

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);

    left = 2;
    right = 5;
    up = 2;
    down = 5;
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    newBoard->up = up;
    newBoard->down = down;
    newBoard->left = left;
    newBoard->right = right;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = left; i <= right; i++) {
        for (int j = up; j <= down; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();

    // Change the valid-move rectangle boundary.
    if (X == left) {
        if (left != 0)
            left--;
    }
    else if (X == right) {
        if (right != 7)
            right++;
    }
    if (Y == up) {
        if (up != 0)
            up--;
    }
    else if (Y == down) {
        if (down != 7)
            down++;
    }

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Flips the color at the given squares specified by the vector change.
 */

void Board::flip_color(vector<int> change)
{
    for (auto i = change.begin(); i != change.end(); i++)
    {
        black.flip(*i);
    }
}

/*
 * Tries to combine checkmove and doMove into one function - an attempt to
 * make it more efficient.
 *
 * Return false if the move is illegal. The board does not change.
 * Return true if the move is legal. The board changes with the given move.
 */

bool Board::makeMove(Move *m, Side side)
{
    bool valid = false;

    // A nullptr move means pass.
    if (m == nullptr) return true;

    // Make sure the square is unoccupied.
    int X = m->getX();
    int Y = m->getY();

    if (occupied(X, Y)) return false;

    // Create vector that will tentatively hold all the captured squares.
    vector<int> change;

    Side other = (side == BLACK) ? WHITE: BLACK;

    // Iterate through every direction.
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X + dx;
            int y = Y + dy;

            // Make sure the pieces along this direction is from the other
            // player.
            if (onBoard(x, y) && get(other, x, y))
            {
                do {
                    change.push_back(x + 8 * y);
                    x += dx;
                    y += dy;
                }
                    while (onBoard(x, y) && get(other, x, y));

                // When the piece along this direction is not from the
                // other player, it better be the current player's piece.
                if (onBoard(x, y) && get(side, x, y))
                {
                    // We have a valid move.
                    valid = true;

                    // Flip all the pieces captured in this valid move.
                    flip_color(change);
                    taken[X + 8 * Y] = 1;
                    if (side == BLACK)
                        black[X + 8 * Y] = 1;

                    // Update the valid move boundary of the board.
                    if (X == left) {
                        if (left != 0)
                            left--;
                    }
                    else if (X == right) {
                        if (right != 7)
                            right++;
                    }
                    if (Y == up) {
                        if (up != 0)
                            up--;
                    }
                    else if (Y == down) {
                        if (down != 7)
                            down++;
                    }
                }
                change.clear();
            }
        }
    }
    return valid;
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * The next few functions just retrieve the private variables up, down,
 * left and right.
 */

int Board::getup() {
    return up;
}

int Board::getdown() {
    return down;
}

int Board::getleft() {
    return left;
}

int Board::getright() {
    return right;
}

void Board::setup(int newup) {
    up = newup;
}

void Board::setdown(int newdown) {
    down = newdown;
}

void Board::setleft(int newleft) {
    left = newleft;
}

void Board::setright(int newright) {
    right = newright;
}

/*
 * Draws the current state of the board out for debugging purposes
 */
/*void Board::draw()
{
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(taken[y + x * 8])
            {
                if(black[y + x * 8])
                {
                    std::cerr << " B ";
                }
                else
                {
                    std::cerr << " W ";
                }
            }
            else
            {
                std::cerr << " * ";
            }
        }

        std::cerr << std::endl;
    }
}*/

void Board::draw()
{
    cout << "\n" << endl;
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(taken[y + x * 8])
            {
                if(black[y + x * 8])
                {
                    std::cout << " B ";
                }
                else
                {
                    std::cout << " W ";
                }
            }
            else
            {
                std::cout << " * ";
            }
        }

        std::cout << std::endl;
    }
}


/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    up = 7;
    down = 0;
    left = 7;
    right = 0;
    for (int i = 0; i < 64; i++) {
        int x = i % 8;
        int y = i / 8;
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);

            // Set the rectangular boundary of valid moves.
            if (y < up)
                up = y;
            if (y > down)
                down = y;
            if (x < left)
                left = x;
            if (x > right)
                right = x;
        } if (data[i] == 'w') {
            taken.set(i);

            // Set the rectangular boundary of valid moves.
            if (y < up)
                up = y;
            if (y > down)
                down = y;
            if (x < left)
                left = x;
            if (x > right)
                right = x;
        }
    }
    if (up != 0)
        up--;
    if (down != 7)
        down++;
    if (left != 0)
        left--;
    if (right != 7)
        right++;
}

/*
 * Calculate the board score for a given side.
 *
 * The scoring method we use is much more sophisticated than counting the
 * difference between white (black) and black (white). We use this naive
 * scoring method when there are no valid moves to play on the board.
 *
 * Otherwise, our scoring highly rewards capturing corners. However, the
 * scoring penalizes pieces that are placed next to the corners (which would
 * make it vulnerable to the opponent capturing the corners).
 *
 * Our scoring rewards placing pieces two spaces removed from the corners
 * (this would be squares
 * (0, 1), (1, 0), (1, 1)
 * (6, 0), (7, 1), (6, 1)
 * (0, 6), (1, 7), (1, 6)
 * (6, 7), (7, 6), (6, 6)
 * The diagonals (1, 1), (6, 1), (1, 6), and (6, 6) are worth less because
 * edges are better.
 */
int Board::board_score(Side side)
{
    // Give scores based on how many moves are left.
    int squares_left = 64 - taken.count();

    int CORNERSCORE;
    int DIAG1SCORE;
    int DIAG2SCORE;
    int EDGE1SCORE;
    int EDGE2SCORE;
    int OTHEREDGESCORE;

    if (squares_left > 20)
    {
        CORNERSCORE = 25;
        DIAG1SCORE = -6;
        DIAG2SCORE = 3;
        EDGE1SCORE = -8;
        EDGE2SCORE = 5;
        OTHEREDGESCORE = 2;
    }
    else
    {
        CORNERSCORE = 15;
        DIAG1SCORE = -5;
        DIAG2SCORE = 1;
        EDGE1SCORE = -6;
        EDGE2SCORE = 2;
        OTHEREDGESCORE = 3;
    }

    // Store whether or not side is black.
    int is_black;
    if (side == BLACK)
    {
        is_black = 1;
    }
    else
    {
        is_black = 0;
    }

    // Count the difference in white and black squares.
    int score = countBlack() - countWhite();

    if (!is_black)
    {
        score *= - 1;
    }

    // If the board is done, return a guaranteed score.
    if (isDone())
    {
        if (score > 0)
        {
            return 1000;
        }
        else if (score == 0)
        {
            return 0;
        }
        else
        {
            return -1000;
        }
    }

    // Otherwise, give scores that reward capturing corners.
    for (int i = 0; i < 4; i++)
    {
        // Be aggressive with the corners
        if (taken[corners[i]])
        {
            int corner_black = black[corners[i]];
            bool has_corner;

            if (is_black == corner_black) {
                score += CORNERSCORE;
                has_corner = true;
            }
            else {
                score -= CORNERSCORE;
                has_corner = false;
            }

            // If we have this corner, set the close edges
            // to normal edge-scores.
            for (int j = 0; j < 2; j++)
            {
                if (taken[edge1[i][j]]) {
                    if (is_black == black[edge1[i][j]]) {
                        if (has_corner)
                            score += OTHEREDGESCORE;
                    }
                    else {
                        if (!has_corner)
                            score -= OTHEREDGESCORE;
                    }
                }
            }

            for (int j = 0; j < 2; j++)
            {
                if (taken[edge2[i][j]]) {
                    if (is_black == black[edge2[i][j]]) {
                        if (has_corner)
                            score += OTHEREDGESCORE;
                    }
                    else {
                        if (!has_corner)
                        {
                            score -= OTHEREDGESCORE;
                        }
                    }
                }
            }
        }

        // Otherwise, try to capture spots that are 2 spots away and
        // penalize for spots that are adjacent to the corner.
        else
        {
            // Score the diagonals near the corners. We want to capture
            // diag2, but avoid diag1.
            if (taken[diag1[i]]) {
                if (is_black == black[diag1[i]])
                    score += DIAG1SCORE;
                else
                    score -= DIAG1SCORE;
            }
            else {
                if (taken[diag2[i]]) {
                    if (is_black == black[diag2[i]])
                        score += DIAG2SCORE;
                    else
                        score -= DIAG2SCORE;
                }
            }

            // Now consider the edges near the corners.
            for (int j = 0; j < 2; j++)
            {
                if (taken[edge1[i][j]]) {
                    if (is_black == black[edge1[i][j]])
                        score += EDGE1SCORE;
                    else
                        score -= EDGE1SCORE;
                }
                else {
                    if (taken[edge2[i][j]]) {
                        if (is_black == black[edge2[i][j]])
                            score += EDGE2SCORE;
                        else
                            score -= EDGE2SCORE;
                    }
                }
            }
        }
    }

    // Do the other edges.
    for (int i = 0; i < 8; i++)
    {
        if (taken[otheredges[i]]) {
            if (is_black == black[otheredges[i]])
                score += OTHEREDGESCORE;
            else
                score -= OTHEREDGESCORE;
        }
    }

    return score;
}
