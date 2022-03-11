#include "simpleRandom.h"

SimpleRandom::SimpleRandom() : Selector(){
}

void SimpleRandom::select(boardType &board) {
    int randomOperatorID = std::rand() % operators.size();
    (*operators[randomOperatorID])(board);

    useCounts[randomOperatorID] ++;
    iterations++;
}

void SimpleRandom::updateState(int change) {}
