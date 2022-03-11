#include "randomDescent.h"

RandomDescent::RandomDescent() : Selector(), lastIterationImprovedObjective(), lastLLHUsed() {}

void RandomDescent::select(boardType &board) {
    //select another heuristic if previous did not produce a better objective score
    if(!lastIterationImprovedObjective){
        lastLLHUsed = std::rand() % operators.size();
    }
    (*operators[lastLLHUsed])(board);

    useCounts[lastLLHUsed]++;
    iterations++;
}

void RandomDescent::updateState(int objectiveChange) {
    lastIterationImprovedObjective = objectiveChange < 0;
}
