#ifndef SUDOKU_OOP_SELECTOR_H
#define SUDOKU_OOP_SELECTOR_H

#include <vector>
#include "../operators/operators.h"

class Selector
{
protected:
    std::vector<void (*)(boardType&)>  operators;

public:
    Selector();
    virtual ~Selector() {}
    virtual void select(boardType &board) = 0;
    virtual void updateState(int objChange) = 0;
};


#endif //SUDOKU_OOP_SELECTOR_H
