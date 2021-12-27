#include "LLH.h"

void neighbourhoodSwap(boardType *board) {
    //CAUTION: CURRENT FILL FUNCTION ALLOWS BLOCKS WITH A SINGLE UNFIXED VALUE, WILL RESULT IN INFINITE LOOP
    int r1, c1, r2, c2, tmp;
    vector<MoveData> moveData;

    do {
        r1 = rand() % board->N;
        c1 = rand() % board->N;
    } while (board->fixed[r1][c1]);

    do {
        r2 = rand() % board->n + r1 / board->n * board->n;
        c2 = rand() % board->n + c1 / board->n * board->n;
    } while (board->fixed[r2][c2] || (r1 == r2 && c1 == c2));


    moveData.emplace_back(r1, c1, board->board[r1][c1]);
    moveData.emplace_back(r2, c2, board->board[r2][c2]);
    board->rememberChange(moveData);

    //std::cout<<"SWAPPING CELLS " << r1 << "," << c1 << " with " << r2 << "," << c2 << std::endl;

    tmp = board->board[r1][c1];
    board->board[r1][c1] = board->board[r2][c2];
    board->board[r2][c2] = tmp;

    //OBJ VALUE ISNT RECALCULATED IMPLICITLY AS IT IS AN EXPENSIVE OPERATION
}

//THINK THOROUHGLY ABOUT THE ROLE OF FIXED CELLS IN THESE LLH

//insert would take a cell, change it's position in a sub block and then shift the non-fixed cell values in-between
// non fixed {4, 6, 7, 8} inserting 8 to 4's location would yield {8, 4, 6, 7}
//           r1        r2

void neighbourhoodInsert(boardType &board) {
    int rSource, cSource, rDest, cDest;
    vector<MoveData> moveData;

    do {
        rSource = rand() % board.N;
        cSource = rand() % board.N;
    } while (board.fixed[rSource][cSource]);

    do {
        rDest = rand() % board.n + rSource / board.n * board.n;
        cDest = rand() % board.n + cSource / board.n * board.n;
    } while (board.fixed[rDest][cDest] || (rSource == rDest && cSource == cDest));

    //rSource = 1, cSource=1, rDest=2, cDest=1;
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
        board.board[r][c] = swapVal;

        moveData.emplace_back(r, c, prev);

        if(r == rDest && c == cDest) break;

        swapVal = prev;
        r = rNext, c = cNext;
    }

    board.rememberChange(moveData);
}

//invert would take two cells that dictate the range of changed cells and inverse their order.
// non fixed {4, 6, 7, 8} inverting cells from 4 to 8  would yield {8, 7, 6, 4}
//THIS IS SIMILAR TO CPOEx, make it so that invert supports "skipping" fixed cells.

void neighbourhoodInvert(boardType &board){
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


}

//Centered Point Oriented Exchange (CPOEx)
// randomly selects a target cell in a subsquare, pairs cells to its left with cells to it's right until it bumps into a fixed cell
// {3, 4, 5, 6, 7, 8, 9} if 3 is fixed and centered point is 6, the resulting exchange is {3, 8, 7, 6, 5, 4, 9} (9 unchanged)
