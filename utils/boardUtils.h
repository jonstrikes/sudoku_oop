#ifndef SUDOKU_OOP_BOARDUTILS_H
#define SUDOKU_OOP_BOARDUTILS_H

#include <vector>

/**
 * Returns the neighbour to the left or right of a given cell within a sub block,
 * shifts to next row if no neighbour within the same row.
 *
 * rightNeighbour will go up a row when reached column boundaries and
 * !rightNeighbour will go down a row when reached column boundary.
 **/
std::pair<int, int> getBlockRowNeighbourCoords(bool rightNeighbour, int r, int c, int n);

/**
 * Returns the upper or lower neighbour of a given cell within a sub block,
 * shifts to next col if no neighbour within the same col.
 *
 * downNeighbour will choose right column when reached row boundaries and
 * !downNeighbour will choose left column when reached row boundary.
 **/
std::pair<int, int> getBlockColNeighbourCoords(bool downNeighbour, int r, int c, int n);
bool areInSameBlock(int r1, int c1, int r2, int c2, int n);
bool isWithinBounds(int r, int c, int N);

#endif //SUDOKU_OOP_BOARDUTILS_H
