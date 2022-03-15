#include "operators.h"

void swap(boardType &board) {
    int r1, c1, r2, c2;

    //find two unique non-fixed cells within the same block
    do {
        r1 = fastrand() % board.N;
        c1 = fastrand() % board.N;
    } while (board.fixed[r1][c1]);
    do {
        r2 = fastrand() % board.n + r1 / board.n * board.n;
        c2 = fastrand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));

    //record data of the two cells before swapping
    vector<MoveData> moveData;
    moveData.emplace_back(r1, c1, board.board[r1][c1], board.rowObjectives[r1], board.colObjectives[c1]);
    moveData.emplace_back(r2, c2, board.board[r2][c2], board.rowObjectives[r2], board.colObjectives[c2]);
    //record state before mutating board
    board.rememberChange(moveData);

    //apply change
    int tmp = board.board[r1][c1];
    board.board[r1][c1] = board.board[r2][c2];
    board.board[r2][c2] = tmp;
}