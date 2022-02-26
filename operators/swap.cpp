#include "operators.h"

void swap(boardType &board) {
    //CAUTION: CURRENT FILL FUNCTION ALLOWS BLOCKS WITH A SINGLE UNFIXED VALUE, WILL RESULT IN INFINITE LOOP
    std::cout << "Swap called" << std::endl;

    int r1, c1, r2, c2, tmp;
    vector<MoveData> moveData;

    do {
        r1 = rand() % board.N;
        c1 = rand() % board.N;
    } while (board.fixed[r1][c1]);

    do {
        r2 = rand() % board.n + r1 / board.n * board.n;
        c2 = rand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));


    moveData.emplace_back(r1, c1, board.board[r1][c1]);
    moveData.emplace_back(r2, c2, board.board[r2][c2]);
    board.rememberChange(moveData);

    //std::cout<<"SWAPPING CELLS " << r1 << "," << c1 << " with " << r2 << "," << c2 << std::endl;

    tmp = board.board[r1][c1];
    board.board[r1][c1] = board.board[r2][c2];
    board.board[r2][c2] = tmp;

    //OBJ VALUE ISNT RECALCULATED IMPLICITLY AS IT IS AN EXPENSIVE OPERATION
}