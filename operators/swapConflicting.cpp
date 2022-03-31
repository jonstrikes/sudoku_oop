#include "operators.h"

void swapConflicting(boardType &board) {
    //get candidates
    std::set<std::pair<uint_fast8_t , uint_fast8_t>> candidates = board.getConflictingCells();

    vector<MoveData> moveData;
    if(candidates.empty()){
        board.rememberChange(moveData);
        return;
    }

    auto it = candidates.begin();
    std::advance(it, fastrand() % candidates.size());
    std::pair<uint_fast8_t , uint_fast8_t> cell = *it;

    int r1 = cell.first, c1 = cell.second, r2, c2;
    do {
        r2 = fastrand() % board.n + r1 / board.n * board.n;
        c2 = fastrand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));

    //record data of the two cells before swapping
    moveData.emplace_back(r1, c1, board.board[r1][c1], board.rowObjectives[r1], board.colObjectives[c1]);
    moveData.emplace_back(r2, c2, board.board[r2][c2], board.rowObjectives[r2], board.colObjectives[c2]);
    //record state before mutating board
    board.rememberChange(moveData);

    //apply change
    int tmp = board.board[r1][c1];
    board.board[r1][c1] = board.board[r2][c2];
    board.board[r2][c2] = tmp;
}