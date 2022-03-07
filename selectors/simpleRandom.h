#ifndef SUDOKU_OOP_SIMPLERANDOM_H
#define SUDOKU_OOP_SIMPLERANDOM_H

#include "../models/selector.h"
#include <random>

class SimpleRandom : public Selector
{
public:
    explicit SimpleRandom();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_SIMPLERANDOM_H
