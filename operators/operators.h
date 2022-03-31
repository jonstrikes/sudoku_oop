#ifndef SUDOKU_OOP_OPERATORS_H
#define SUDOKU_OOP_OPERATORS_H

#include "../board.h"
#include "../utils/generalUtils.h"
#include "../utils/boardUtils.h"

void swap(boardType &board);
void insert(boardType &board);
void invert(boardType &board);
void cpoExchange(boardType &board);

void insertSym(boardType &board);
void invertSym(boardType &board);

void swapConflicting(boardType &board);

#endif //SUDOKU_OOP_OPERATORS_H
