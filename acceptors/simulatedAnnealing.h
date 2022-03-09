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

    double temperature;
    double initialTemperature;
    int iterationCount;
    int iterationLimit;
    int worseningIterationCount;
    int improvingIterationCount;
    int restartCount;
    int startingObjective;

    bool isStuck;

    double calculateInitialTemperature(boardType &board, Selector &selector);
    int calculateIterationLimit(boardType &board);


public:
    explicit SimulatedAnnealing(boardType &board, Selector &selector);
    int process(boardType &board) override;
};

#endif //SUDOKU_OOP_SIMULATEDANNEALING_H
