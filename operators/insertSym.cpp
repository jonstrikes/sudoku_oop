#include "operators.h"

void insertSym(boardType &board) {
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

    //determine the direction which cells will be shifted for the insertion
    bool isBackwardInsert = (cDest > cSource || cDest == cSource && rDest > rSource);

    int r = rSource, c = cSource, swapVal = board.board[rDest][cDest], prev, rNext, cNext;
    while (true) {
        //get next coordinates based on if up or down rotation
        if (isBackwardInsert) {
            //check if need to move to next col
            if ((r + 1) % board.n == 0) {
                rNext = r + 1 - board.n;
                cNext = c + 1;
            } else rNext = r + 1, cNext = c;

        } else {
            //check if need to move to previous col
            if (r % board.n == 0) {
                rNext = r - 1 + board.n;
                cNext = c - 1;
            } else rNext = r - 1, cNext = c;
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

