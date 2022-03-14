#include "operators.h"

void insert(boardType &board) {
    int rSource, cSource, rDest, cDest;
    vector<MoveData> moveData;

//    std::cout << "Insert called" << std::endl;

    do {
        rSource = rand() % board.N;
        cSource = rand() % board.N;
    } while (board.fixed[rSource][cSource]);

    do {
        rDest = rand() % board.n + rSource / board.n * board.n;
        cDest = rand() % board.n + cSource / board.n * board.n;
    } while (board.fixed[rDest][cDest] || (rSource == rDest && cSource == cDest));

    bool isBackwardInsert = (rDest > rSource || rDest == rSource && cDest > cSource);

    //std::cout << "Backwards?: " << isBackwardInsert << " swap range: " << cDest-cSource+board.n*(rDest-rSource)<< " " << rSource << ":" << cSource << " with " << rDest << ":" << cDest << std::endl;

    int r = rSource, c = cSource, swapVal = board.board[rDest][cDest], prev, rNext, cNext;
    //DOES NOT HANDLE BLOCKS WHERE ONLY ONE CELL IS NOT FIXED
    while(true){
        //get next coordinates based on if right or left rotation
        if(isBackwardInsert){
            //check if need to move to next row
            if((c+1) % board.n == 0){
                cNext = c+1 - board.n;
                rNext = r+1;
            }
            else cNext = c+1, rNext=r;

        }
        else{
            //check if need to move to previous row
            if(c % board.n == 0){
                cNext = c-1 + board.n;
                rNext = r-1;
            }
            else cNext=c-1, rNext=r;
        }

        //std::cout << "next coords "<< rNext << ":" << cNext << std::endl;

        if(board.fixed[r][c]){
            r = rNext, c = cNext;
            continue;
        }

        prev = board.board[r][c];
        moveData.emplace_back(r, c, prev, board.rowObjectives[r], board.colObjectives[c]);
        board.board[r][c] = swapVal;

        if(r == rDest && c == cDest) break;

        swapVal = prev;
        r = rNext, c = cNext;
    }

    board.rememberChange(moveData);
}

