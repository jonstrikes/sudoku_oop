#include "reinforcementLearning.h"

ReinforcementLearning::ReinforcementLearning(int utilityUpperBoundFactor, double initialUtilityFactor)
        : Selector(), lastLLHUsed(), lowerUtilityBound(), utilityValues(operators.size()) {
    //calculate upper bound of a utility value
    upperUtilityBound = operators.size() * 5 * utilityUpperBoundFactor;

    //initialise utility values of each operator
    for (int i = 0; i < operators.size(); i++) {
        utilityValues[i] = upperUtilityBound * initialUtilityFactor;
    }
}

void ReinforcementLearning::select(boardType &board) {
    //identify operator(s) with maximal utility value(s)
    std::vector<int> ids;
    auto it = std::max_element(std::begin(utilityValues), std::end(utilityValues));
    while (it != std::end(utilityValues))
    {
        ids.push_back(std::distance(std::begin(utilityValues), it));
        it = std::find(std::next(it), std::end(utilityValues), *it);
    }

    //randomly select operator with maximal utility
    lastLLHUsed = ids[fastrand() % ids.size()];

    //apply operator
    (*operators[lastLLHUsed])(board);

    useCounts[lastLLHUsed]++;
    iterations++;
}

void ReinforcementLearning::updateState(int objectiveChange) {
    if(objectiveChange < 0){
        //reward operator if it improved objective
        utilityValues[lastLLHUsed] = std::min(utilityValues[lastLLHUsed] * 2, (double)upperUtilityBound);
    } else {
        //punish operator if it worsened objective
        utilityValues[lastLLHUsed] = std::max(std::sqrt(utilityValues[lastLLHUsed]), (double)lowerUtilityBound);
    }
}