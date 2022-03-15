#ifndef SUDOKU_OOP_RANDOMDESCENT_H
#define SUDOKU_OOP_RANDOMDESCENT_H

#include "../models/selector.h"

class RandomDescent : public Selector
{
private:
    bool lastIterationImprovedObjective;
    uint_fast8_t lastLLHUsed;
public:
    explicit RandomDescent();
    void select(boardType &board) override;
    void updateState(int change) override;
};


#endif //SUDOKU_OOP_RANDOMDESCENT_H
