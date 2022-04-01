#ifndef SUDOKU_OOP_SELECTOR_H
#define SUDOKU_OOP_SELECTOR_H

#include <vector>
#include "../operators/operators.h"

class Selector
{
protected:
    //the set of operator functions in use
    std::vector<void (*)(boardType&)>  operators;
    std::vector<int> useCounts;
    std::vector<double> timesSpent;
    int iterations;

    //centralised method for sub-classes that take care of tracking operators and logging changes
    void applyOperator(boardType &board, int operatorId);

public:
    Selector();
    virtual ~Selector() {}

    //selects the next operator to be applied based on some strategy
    virtual void select(boardType &board) = 0;
    virtual void updateState(int objChange) = 0;

    //the number of iterations passed since initialisation
    int getIterations();

    //general data collected for all implementing selectors
    void printLog();
    std::string getLog();
};


#endif //SUDOKU_OOP_SELECTOR_H
