#include "randomDescent.h"

RandomDescent::RandomDescent() : Selector() {
    currentId = fastrand() % operators.size();
}

void RandomDescent::select(boardType &board) {
    (*operators[currentId])(board);

    useCounts[currentId]++;
    iterations++;
}

void RandomDescent::updateState(int change) {
    //choose next operator if last change did not improve objective score
    if(change >= 0){
        currentId = fastrand() % operators.size();
    }
}
