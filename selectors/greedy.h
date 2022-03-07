#ifndef SUDOKU_OOP_GREEDY_H
#define SUDOKU_OOP_GREEDY_H

#include "../models/selector.h"
#include <limits>
#include "../board.h"
#include "../Solver.h"

class Greedy : public Selector
{
private:
    int bestOperator;
    int bestScore;
public:
    explicit Greedy();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_GREEDY_H
