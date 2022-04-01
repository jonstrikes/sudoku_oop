#ifndef SUDOKU_OOP_SELECTOR_H
#define SUDOKU_OOP_SELECTOR_H

#include <vector>
#include "../operators/operators.h"

class Selector
{
protected:
    std::vector<void (*)(boardType&)>  operators;
    std::vector<int> useCounts;
    std::vector<double> timesSpent;
    int iterations;

    void applyOperator(boardType &board, int operatorId);

public:
    Selector();
    virtual ~Selector() {}
    virtual void select(boardType &board) = 0;
    virtual void updateState(int objChange) = 0;
    void printLog();
    int getIterations();

    std::string getLog();
};


#endif //SUDOKU_OOP_SELECTOR_H
