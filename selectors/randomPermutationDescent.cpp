#include "randomPermutationDescent.h"

RandomPermutationDescent::RandomPermutationDescent() : Selector(){
    for(int i=0; i<operators.size(); i++) { permutation.push_back(i);}

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(permutation), std::end(permutation), rng);

    currentId = permutation[0];
}

void RandomPermutationDescent::select(boardType &board) {
    (*operators[permutation[currentId]])(board);
}

void RandomPermutationDescent::updateState(int change) {
    if(change < 0)
        return;
    //choose next operator if last change did not improve objective score
    currentId = (currentId + 1) % operators.size();
}
