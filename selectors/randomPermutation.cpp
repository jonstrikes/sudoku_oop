#include "randomPermutation.h"

RandomPermutation::RandomPermutation() : Selector(){
    for(int i=0; i<operators.size(); i++) { permutation.push_back(i);}

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(permutation), std::end(permutation), rng);

    currentId = 0;
}

void RandomPermutation::select(boardType &board) {
    (*operators[currentId])(board);

    useCounts[permutation[currentId]] ++;
    iterations++;
}

void RandomPermutation::updateState(int change) {
    currentId = (currentId + 1) % operators.size();
}
