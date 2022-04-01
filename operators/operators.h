#ifndef SUDOKU_OOP_OPERATORS_H
#define SUDOKU_OOP_OPERATORS_H

#include "../board.h"
#include "../utils/generalUtils.h"
#include "../utils/boardUtils.h"

/**
 * Low level heuristics are used to explore the search space by mutating the current solution.
 *
 * Insert, Invert and CpoExchange low level heuristics are used from
 *  "A variable neighborhood search for solving sudoku puzzles" (Hamza, Sevkli)
 *
 * swapConflicting was inspired by the Constraint Programming section from
 *  "A Hybrid Approach for the Sudoku problem: Using Constraint Programming in Iterated Local Search" (Musliu, Winter)
 *
 * InsertSym and InvertSym were implemented after observing Insert and Invert.
 *
 **/

//swaps two non-fixed cells within a sub-block randomly
void swap(boardType &board);

//shifts a sequence of non-fixed cells within a sub-block if possible. Iterates cells row by row
void insert(boardType &board);
//inverts a sequence of non-fixed cells within a sub-block if possible. Iterates cells row by row
void invert(boardType &board);

//inverts pairs of non-fixed cells around a randomly selected cell within a sub-block if possible. Iterates row by row
void cpoExchange(boardType &board);

//shifts a sequence of non-fixed cells within a sub-block if possible. Iterates cells column by column
void insertSym(boardType &board);
//inverts a sequence of non-fixed cells within a sub-block if possible. Iterates cells column by column
void invertSym(boardType &board);

//swaps a non-fixed conflicting cell with any non-fixed cell within a sub-block randomly if possible.
void swapConflicting(boardType &board);

#endif //SUDOKU_OOP_OPERATORS_H
