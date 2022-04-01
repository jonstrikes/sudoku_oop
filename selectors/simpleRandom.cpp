#include "simpleRandom.h"

SimpleRandom::SimpleRandom() : Selector(){
}

void SimpleRandom::select(boardType &board) {
    applyOperator(board, fastrand() % operators.size());
}

void SimpleRandom::updateState(int change) {}
