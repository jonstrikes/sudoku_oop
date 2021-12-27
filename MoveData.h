#ifndef SUDOKU_OOP_MOVEDATA_H
#define SUDOKU_OOP_MOVEDATA_H

#include <map>

struct MoveData{
private:
    std::array<int, 3> move{};
public:
    MoveData(int row, int col, int value);
    int getRow();
    int getCol();
    int getVal();
};

#endif //SUDOKU_OOP_MOVEDATA_H
