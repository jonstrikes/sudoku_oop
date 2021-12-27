#include "MoveData.h"


MoveData::MoveData(int row, int col, int value) {
    move = {row, col, value};
}

int MoveData::getRow() { return this->move[0]; }

int MoveData::getCol() { return this->move[1]; }

int MoveData::getVal() { return this->move[2]; }
