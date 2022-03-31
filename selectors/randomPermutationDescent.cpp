#include "randomPermutationDescent.h"

RandomPermutationDescent::RandomPermutationDescent() : Selector(){
    //generate ids for operators
    for(int i=0; i<operators.size(); i++) { permutation.push_back(i);}

    //shuffle ids instead of the original operator vector
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(permutation), std::end(permutation), rng);

    //point to first element in permutation
    currentId = 0;
}

void RandomPermutationDescent::select(boardType &board) {
    (*operators[permutation[currentId]])(board);

    useCounts[permutation[currentId]]++;
    iterations++;
}

void RandomPermutationDescent::updateState(int change) {
    //choose next operator if last change did not improve objective score
    if(change >= 0){
        currentId = (currentId + 1) % operators.size();
    }
}
