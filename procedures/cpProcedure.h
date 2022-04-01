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

    const int WORSENING_CYCLES_LIMIT = 50;
    const double WORSENING_CYCLES_FACTOR = 1.0;
    const double CYCLE_ITERATIONS_FACTOR = 1.0;

    const double RESET_INITIAL = 1.0;
    const double RESET_MIN = 0.1;
    const double RESET_MAX = 1.0;
    const double RESET_ALPHA = 0.5;
    const double RESET_BETA = 0.2;

    double resetFactor;
    int cyclesWithoutImprovement;
    int currentCycleLength;
    int iteration;
    const vector<vector<bool>> originalFixed;
    int bestCycleObjective;
public:
    CpProcessor(boardType &board, Acceptor *&acceptor, int WORSENING_CYCLES_LIMIT, double CYCLE_ITERATIONS_FACTOR,
                double WORSENING_CYCLES_FACTOR, double RESET_INITIAL,
                double RESET_MIN, double RESET_MAX, double RESET_ALPHA, double RESET_BETA);

    void run();
};

void cpProcedure(boardType &board, double reset);

#endif //SUDOKU_OOP_CPPROCEDURE_H
