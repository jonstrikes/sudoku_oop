#ifndef SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H
#define SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H

#include "../models/acceptor.h"

class AdaptiveIterationLimitedThresholdAccepting : public Acceptor
{
private:
    int unfixedCount;

    int iteration;
    int w_iterations;
    int k;

    double e;
    int wIterationThreshold;

    void resetParameters();
public:
    explicit AdaptiveIterationLimitedThresholdAccepting(boardType &board);
    int process(boardType &board) override;
};

#endif //SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H
