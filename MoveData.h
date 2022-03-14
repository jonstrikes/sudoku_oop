#ifndef SUDOKU_OOP_MOVEDATA_H
#define SUDOKU_OOP_MOVEDATA_H

#include <map>

struct MoveData{
public:
    MoveData(int row, int col, int value) : row(row), col(col), val(value) {};
    int row;
    int col;
    int val;
};

#endif //SUDOKU_OOP_MOVEDATA_H
