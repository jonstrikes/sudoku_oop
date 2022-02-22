#ifndef SUDOKU_OOP_LLHMANAGER_H
#define SUDOKU_OOP_LLHMANAGER_H

#include "board.h"
#include <vector>
#include <tuple>
#include <cmath>

void neighbourhoodSwap(boardType &board);
void neighbourhoodInsert(boardType &board);
void neighbourhoodInvert(boardType &board);
void neighbourhoodCPOEx(boardType &board);

#endif //SUDOKU_OOP_LLHMANAGER_H
