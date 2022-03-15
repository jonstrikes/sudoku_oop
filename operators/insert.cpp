#include "operators.h"

void insert(boardType &board) {
    int rSource, cSource, rDest, cDest;
    vector<MoveData> moveData;

    //find two unique non-fixed cells within the same block
    do {
        rSource = fastrand() % board.N;
        cSource = fastrand() % board.N;
    } while (board.fixed[rSource][cSource]);
    do {
        rDest = fastrand() % board.n + rSource / board.n * board.n;
        cDest = fastrand() % board.n + cSource / board.n * board.n;
    } while (board.fixed[rDest][cDest] || (rSource == rDest && cSource == cDest));

    //determine if the source cell comes before the destination cell
    bool isBackwardInsert = (rDest > rSource || rDest == rSource && cDest > cSource);

    int r = rSource, c = cSource, swapVal = board.board[rDest][cDest], prev, rNext, cNext;
    while (true) {
        //get next coordinates based on if right or left rotation
        if (isBackwardInsert) {
            //check if need to move to next row
            if ((c + 1) % board.n == 0) {
                cNext = c + 1 - board.n;
                rNext = r + 1;
            } else cNext = c + 1, rNext = r;

        } else {
            //check if need to move to previous row
            if (c % board.n == 0) {
                cNext = c - 1 + board.n;
                rNext = r - 1;
            } else cNext = c - 1, rNext = r;
        }
        //skip fixed cells
        if (board.fixed[r][c]) {
            r = rNext, c = cNext;
            continue;
        }

        //record data of the cell before insertion
        prev = board.board[r][c];
        moveData.emplace_back(r, c, prev, board.rowObjectives[r], board.colObjectives[c]);
        board.board[r][c] = swapVal;

        //finish when destination is reached
        if (r == rDest && c == cDest) break;

        //shift current cell
        swapVal = prev;
        r = rNext, c = cNext;
    }

    //record starting state
    board.rememberChange(moveData);
}

