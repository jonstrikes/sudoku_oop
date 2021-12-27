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

    bool isBackwardInsert = (r2 > r1 || r2 == r1 && c2 > c1);

    std::cout << "Backwards?: " << isBackwardInsert << " " << r1 << ":" << c1 << " with " << r2 << ":" << c2 << std::endl;

    int r = r1, c = c1, swapVal = board.board[r2][c2], rNext, cNext;
    //DOES NOT HANDLE BLOCKS WHERE ONLY ONE CELL IS NOT FIXED
    while(true){
        //next cell coordinates depending on if rotation is right or left
        if(isBackwardInsert){
            cNext = (c+1)%board.n + c / board.n * board.n;
            rNext = (r%board.n) + (cNext < c) + r / board.n * board.n;
        }
        else{
            cNext = c / board.n * board.n + (c-1)%board.n;
            rNext = r / board.n * board.n + (r%board.n) - (cNext > c);
        }

        if(board.fixed[r][c]){
            r = rNext, c = cNext;
            continue;
        }

        int temp = board.board[r][c];
        board.board[r][c] = swapVal;

        moveData.emplace_back(r, c, temp);

        if(r == r2 && c == c2) break;

        swapVal = temp;
        r = rNext, c = cNext;
    }

    board.rememberChange(moveData);
}

//void rightRotate(boardType &board, vector<MoveData> &moveData, int r, int c, int destVal, int r2, int c2){
//    //next cell coordinates, going forwards
//    int cNext = (c+1)%board.n + c / board.n * board.n;
//    int rNext = (r%board.n) + (cNext < c) + r / board.n * board.n;
//
//    if(board.fixed[r][c]){
//        rightRotate(board, moveData, rNext, cNext, destVal, r2, c2);
//        return;
//    }
//
//    int temp = board.board[r][c];
//    board.board[r][c] = destVal;
//
//    moveData.emplace_back(r, c, temp);
//    if(r == r2 && c == c2) return;
//    rightRotate(board, moveData, rNext, cNext, temp, r2, c2);
//}
//
//void leftRotate(boardType &board, vector<MoveData> &moveData, int r, int c, int destVal, int r2, int c2){
//    //next cell coordinates, going backwards
//    int cNext = c / board.n * board.n + (c-1)%board.n;
//    int rNext = r / board.n * board.n + (r%board.n) - (cNext > c);
//
//    if(board.fixed[r][c]){
//        leftRotate(board, moveData, rNext, cNext, destVal, r2, c2);
//        return;
//    }
//
//    int temp = board.board[r][c];
//    board.board[r][c] = destVal;
//
//    moveData.emplace_back(r, c, temp);
//    if(r == r2 && c == c2) return;
//    leftRotate(board, moveData, rNext, cNext, temp, r2, c2);
//
//}

//invert would take two cells that dictate the range of changed cells and inverse their order.
// non fixed {4, 6, 7, 8} inverting cells from 4 to 8  would yield {8, 7, 6, 4}
//THIS IS SIMILAR TO CPOEx, make it so that invert supports "skipping" fixed cells.

//Centered Point Oriented Exchange (CPOEx)
// randomly selects a target cell in a subsquare, pairs cells to its left with cells to it's right until it bumps into a fixed cell
// {3, 4, 5, 6, 7, 8, 9} if 3 is fixed and centered point is 6, the resulting exchange is {3, 8, 7, 6, 5, 4, 9} (9 unchanged)
