#ifndef SUDOKU_OOP_GREEDY_H
#define SUDOKU_OOP_GREEDY_H

#include "../models/selector.h"
#include <limits>
#include "../board.h"

class Greedy : public Selector
{
private:
    uint_fast8_t bestOperator;
    uint_fast8_t bestScore;
public:
    explicit Greedy();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_GREEDY_H
