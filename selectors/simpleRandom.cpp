#include "simpleRandom.h"

SimpleRandom::SimpleRandom() : Selector(){
}

void SimpleRandom::select(boardType &board) {
    int randomOperatorID = std::rand() % operators.size();
    //(*operators[randomOperatorID])(board);
    (*operators[randomOperatorID])(board);
}

void SimpleRandom::updateState(int change) {}
