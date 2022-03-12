#include "simpleRandom.h"

SimpleRandom::SimpleRandom() : Selector(){
}

void SimpleRandom::select(boardType &board) {
    int randomOperatorID = std::rand() % operators.size();

        clock_t s1 = clock();
    (*operators[randomOperatorID])(board);
        timesSpent[randomOperatorID] += (double)(clock() - s1);

    useCounts[randomOperatorID] ++;
    iterations++;
}

void SimpleRandom::updateState(int change) {}
