#include "LLH.h"

void neighbourhoodSwap(boardType *board, std::vector<std::tuple<int, int, int>> *changedCells) {
    //CAUTION: CURRENT FILL FUNCTION ALLOWS BLOCKS WITH A SINGLE UNFIXED VALUE, WILL RESULT IN INFINITE LOOP
    int r1, c1, r2, c2, tmp;

    do {
        r1 = rand() % board->N;
        c1 = rand() % board->N;
    } while (board->fixed[r1][c1]);

    do {
        r2 = rand() % board->n + r1 / board->n * board->n;
        c2 = rand() % board->n + c1 / board->n * board->n;
    } while (board->fixed[r2][c2] || (r1 == r2 && c1 == c2));

    changedCells->push_back(std::make_tuple(r1, c1, board->board[r1][c1]));
    changedCells->push_back(std::make_tuple(r2, c2, board->board[r2][c2]));

    std::cout<<"SWAPPING CELLS " << r1 << "," << c1 << " with " << r2 << "," << c2 << std::endl;

    tmp = board->board[r1][c1];
    board->board[r1][c1] = board->board[r2][c2];
    board->board[r2][c2] = tmp;
}
