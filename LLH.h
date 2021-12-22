#ifndef SUDOKU_OOP_LLHMANAGER_H
#define SUDOKU_OOP_LLHMANAGER_H

#include "board.h"
#include <vector>
#include <tuple>
void neighbourhoodSwap(boardType *board, std::vector<std::tuple<int, int, int>> *changedCells);

#endif //SUDOKU_OOP_LLHMANAGER_H
