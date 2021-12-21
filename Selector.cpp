#include "Selector.h"

Selector::Selector(enum selectionMethod method) {
    hasImprovedSolution = false;
    selectionMethod = method;
    //initialise permutation
    for (int i = 0; i < LAST_LLH; i++) permutation.push_back(static_cast<LLH>(i));
    std::random_shuffle(permutation.begin(), permutation.end());
    //an id to track the current LLH for RP and RPD
    currentLLHid = 0;
}

int Selector::select() {
    switch (selectionMethod) {
        case SR:
            return simpleRandom();
        case RD:
            return randomDescent();
        case RP:
            return randomPermutation();
        case RPD:
            return randomPermutationDescent();
        case GR:
            return greedySelection();
        default:
            break;
    }
}

int Selector::simpleRandom() {
    //randomly select low level heuristic
    LLH currentLLH = static_cast<LLH>(rand() % LAST_LLH);
    //should undo last move if it sucks??
    return applyLLH(currentLLH);
}

int Selector::randomDescent() {
    //randomly select low level heuristic
    LLH currentLLH = static_cast<LLH>(rand() % LAST_LLH);
    //reapply if solution is better
    int totalChange = 0;
    do {
        totalChange += applyLLH(currentLLH);
    } while (hasImprovedSolution);

    //how do I undo moves?

    return totalChange;
}

int Selector::randomPermutation() {
    int change = applyLLH(permutation[currentLLHid]);
    //should undo last move if it sucks??
    currentLLHid++;
    return change;
}

int Selector::randomPermutationDescent() {
    int totalChange = 0;
    do {
        totalChange += applyLLH(permutation[currentLLHid]);
    } while (hasImprovedSolution);
    //should undo last move if it sucks??
    currentLLHid++;
    return totalChange;
}

int Selector::greedySelection() {
    //DUNNO IF THIS IS POSSIBLE
    //apply all llh and choose the one that improves upon the solution the most
    for (int i = 0; i < LAST_LLH; i++) {
        //do
        //undo
    }
}

int Selector::applyLLH(LLH selectedHeuristic) {
    switch (selectedHeuristic) {
        case NEIGHBOURHOOD_SWAP:
            //neighbourhoodSwap();
            break;
        case someOtherLLH:
            //someOtherLLH();
            break;
        default:
            break;
    }
    int change = recalcObj();
    hasImprovedSolution = change < 0;
    return change;
}
