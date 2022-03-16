#include "simpleRandom.h"

SimpleRandom::SimpleRandom() : Selector(){
}

void SimpleRandom::select(boardType &board) {
    uint_fast8_t randomOperatorID = fastrand() % operators.size();
    //randomOperatorID = 0;

        clock_t s1 = clock();
    (*operators[randomOperatorID])(board);
        timesSpent[randomOperatorID] += (double)(clock() - s1);

    useCounts[randomOperatorID] ++;
    iterations++;
}

void SimpleRandom::updateState(int change) {}
