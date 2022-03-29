#ifndef SUDOKU_OOP_SIMULATEDANNEALING_H
#define SUDOKU_OOP_SIMULATEDANNEALING_H

#include "../models/acceptor.h"
#include "../models/selector.h"
#include <numeric>

class SimulatedAnnealing : public Acceptor
{
private:
    const int SAMPLE_SIZE = 100;
    const int WORSENING_ITERATION_LIMIT = 30;
    const double COOL_RATE = 0.99;
    const double TEMPERATURE_THRESHOLD = 0.000001;
    Selector &selector;

    double temperature;
    double initialTemperature;
    int iterationCount;
    int iterationLimit;
    int worseningIterationCount;
    int iterationCycle;
    int restartCount;
    int startingObjective;

    bool isStuck;

    double calculateTemperature(boardType &board);
    int calculateIterationLimit(boardType &board);

public:
    explicit SimulatedAnnealing(boardType &board, Selector &selector);
    int recalculateObjective(boardType &board) override;
    int process(boardType &board) override;
};

#endif //SUDOKU_OOP_SIMULATEDANNEALING_H
