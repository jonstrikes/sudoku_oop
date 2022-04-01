#ifndef SUDOKU_OOP_CPPROCEDURE_H
#define SUDOKU_OOP_CPPROCEDURE_H

#include "../utils/generalUtils.h"
#include "../board.h"
#include "../models/acceptor.h"

#include <set>
#include <vector>

class CpProcessor {
private:
    boardType &board;
    Acceptor &acceptor;

    const int WORSENING_CYCLES_LIMIT;
    const double WORSENING_CYCLES_FACTOR;
    const double CYCLE_ITERATIONS_FACTOR;
    const double LOG_CYCLE_LIMIT;

    const double RESET_INITIAL;
    const double RESET_MIN;
    const double RESET_MAX;
    const double RESET_ALPHA;
    const double RESET_BETA;

    double resetFactor;
    int cyclesWithoutImprovement;
    int currentCycleLength;
    int iteration;
    const vector<vector<bool>> originalFixed;
    int bestCycleObjective;
public:
    CpProcessor(boardType &board, Acceptor *&acceptor, int WORSENING_CYCLES_LIMIT,
                double CYCLE_ITERATIONS_FACTOR, int LOG_CYCLE_LIMIT,
                double WORSENING_CYCLES_FACTOR, double RESET_INITIAL,
                double RESET_MIN, double RESET_MAX, double RESET_ALPHA, double RESET_BETA);

    void run();
};

void cpProcedure(boardType &board, double reset);

#endif //SUDOKU_OOP_CPPROCEDURE_H
