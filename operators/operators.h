#ifndef SUDOKU_OOP_OPERATORS_H
#define SUDOKU_OOP_OPERATORS_H

#include "../board.h"
#include "../utils/generalUtils.h"
#include "../utils/boardUtils.h"

//swaps two non-fixed cells within a sub-block randomly
void swap(boardType &board);

//shifts a sequence of non-fixed cells within a sub-block if possible. Iterates row by row
void insert(boardType &board);
//inverts a sequence of non-fixed cells within a sub-block if possible. Iterates row by row
void invert(boardType &board);
//inverts pairs of non-fixed cells around a randomly selected cell within a sub-block if possible. Iterates row by row
void cpoExchange(boardType &board);

//shifts a sequence of non-fixed cells within a sub-block if possible. Iterates column by column
void insertSym(boardType &board);
//inverts a sequence of non-fixed cells within a sub-block if possible. Iterates column by column
void invertSym(boardType &board);

//swaps a non-fixed conflicting cell with any non-fixed cell within a sub-block randomly if possible.
void swapConflicting(boardType &board);

#endif //SUDOKU_OOP_OPERATORS_H
