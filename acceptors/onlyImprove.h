#ifndef SUDOKU_OOP_ONLYIMPROVE_H
#define SUDOKU_OOP_ONLYIMPROVE_H

#include "../models/acceptor.h"

class OnlyImprove : public Acceptor
{
public:
    explicit OnlyImprove(boardType &board);
    int process(boardType &board) override;
    void printShortLog() override;
};

#endif //SUDOKU_OOP_ONLYIMPROVE_H