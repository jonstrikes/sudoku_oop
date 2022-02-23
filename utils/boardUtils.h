#ifndef SUDOKU_OOP_BOARDUTILS_H
#define SUDOKU_OOP_BOARDUTILS_H

#include <vector>

std::pair<int, int> getNeighbourCoords(bool rightNeighbour, int r, int c, int n);
bool areInSameBlock(int r1, int c1, int r2, int c2, int n);
bool isWithinBounds(int r, int c, int N);

#endif //SUDOKU_OOP_BOARDUTILS_H
