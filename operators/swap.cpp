#include "operators.h"

void swap(boardType &board) {
    //std::cout << "Swap called" << std::endl;

    int r1, c1, r2, c2, tmp;
    vector<MoveData> moveData;

    do {
        r1 = fastrand() % board.N;
        c1 = fastrand() % board.N;
    } while (board.fixed[r1][c1]);

    do {
        r2 = fastrand() % board.n + r1 / board.n * board.n;
        c2 = fastrand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));


    moveData.emplace_back(r1, c1, board.board[r1][c1], board.rowObjectives[r1], board.colObjectives[c1]);
    moveData.emplace_back(r2, c2, board.board[r2][c2], board.rowObjectives[r2], board.colObjectives[c2]);
    //try calling board.moveRecord.recordChange();
    board.rememberChange(moveData);

    //std::cout<<"SWAPPING CELLS " << r1 << "," << c1 << " with " << r2 << "," << c2 << std::endl;

    tmp = board.board[r1][c1];
    board.board[r1][c1] = board.board[r2][c2];
    board.board[r2][c2] = tmp;
}