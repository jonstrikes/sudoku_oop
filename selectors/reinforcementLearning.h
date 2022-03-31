#ifndef SUDOKU_OOP_REINFORCEMENTLEARNING_H
#define SUDOKU_OOP_REINFORCEMENTLEARNING_H

#include "../models/selector.h"

class ReinforcementLearning : public Selector
{
private:
    vector<double> utilityValues;

    int lastLLHUsed;
    int lowerUtilityBound;
    int upperUtilityBound;

public:
    explicit ReinforcementLearning(int utilityUpperBoundFactor, double initialUtilityFactor);
    void select(boardType &board) override;
    void updateState(int change) override;
};


#endif //SUDOKU_OOP_REINFORCEMENTLEARNING_H
