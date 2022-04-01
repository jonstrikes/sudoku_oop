#ifndef SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H
#define SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H

#include "../models/acceptor.h"

class AdaptiveIterationLimitedThresholdAccepting : public Acceptor {
private:
    const int W_ITERATION_THRESHOLD;
    const double K_FACTOR;
    const double E_INITIAL;
    const double E_FACTOR;

    int iteration;
    int w_iterations;
    int k;

    double e;

    void resetParameters();

public:
    explicit AdaptiveIterationLimitedThresholdAccepting(boardType &board, int W_ITERATION_THRESHOLD, double K_FACTOR,
                                                        double E_INITIAL, double E_FACTOR);

    int process(boardType &board) override;

    void printShortLog() override;
};

#endif //SUDOKU_OOP_ADAPTIVEITERATIONLIMITEDTHRESHOLDACCEPTING_H
