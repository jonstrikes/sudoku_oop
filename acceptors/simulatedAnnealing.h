#ifndef SUDOKU_OOP_SIMULATEDANNEALING_H
#define SUDOKU_OOP_SIMULATEDANNEALING_H

#include "../models/acceptor.h"
#include "../models/selector.h"
#include <numeric>

/**
 * This selection method was adapted following the successful metaheuristic approach from
 *  "Metaheuristics can solve sudoku puzzles"
 *  (Lewis)
 **/
class SimulatedAnnealing : public Acceptor
{
private:
    const int SAMPLE_SIZE;
    const int NON_IMPROVING_CYCLE_LIMIT;
    const double COOL_RATE;
    const double TEMPERATURE_THRESHOLD;

    const double TEMPERATURE_FACTOR;
    const double CYCLE_LENGTH_FACTOR;

    Selector &selector;

    double temperature;
    int iterationCount;
    int iterationLimit;
    int cyclesWithoutImprovement;
    int iterationCycle;
    int restartCount;
    int startingObjective;

    bool isStuck;

    double calculateTemperature(boardType &board);
    int calculateIterationLimit(boardType &board);

public:
    explicit SimulatedAnnealing(boardType &board, Selector &selector,
                                double COOL_RATE,double TEMPERATURE_THRESHOLD, double TEMPERATURE_FACTOR,
                                int SAMPLE_SIZE, int NON_IMPROVING_CYCLE_LIMIT, double CYCLE_LENGTH_FACTOR);

    int resetState(boardType &board) override;
    int process(boardType &board) override;
    void printShortLog() override;
};

#endif //SUDOKU_OOP_SIMULATEDANNEALING_H
