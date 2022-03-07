#include "randomPermutation.h"

RandomPermutation::RandomPermutation() : Selector(){
    for(int i=0; i<operators.size(); i++) { permutation.push_back(i);}

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(permutation), std::end(permutation), rng);

    currentId = permutation[0];
}

void RandomPermutation::select(boardType &board) {
    (*operators[currentId])(board);
}

void RandomPermutation::updateState(int change) {
    currentId = (currentId + 1) % operators.size();
}
