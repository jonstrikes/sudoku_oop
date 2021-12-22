#include "MoveData.h"


MoveData::MoveData(int row, int col, int value) {
    move = std::make_tuple(row, col, value);
}

int MoveData::getRow() { return std::get<0>(this->move); }

int MoveData::getCol() { return std::get<1>(this->move); }

int MoveData::getVal() { return std::get<2>(this->move); }
