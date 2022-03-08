#ifndef SUDOKU_OOP_IMPROVEOREQUAL_H
#define SUDOKU_OOP_IMPROVEOREQUAL_H

#include "../models/acceptor.h"

class ImproveOrEqual : public Acceptor
{
public:
    explicit ImproveOrEqual(boardType &board);
    int process(boardType &board) override;
};

#endif //SUDOKU_OOP_IMPROVEOREQUAL_H
