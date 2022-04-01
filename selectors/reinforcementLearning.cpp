#include "reinforcementLearning.h"

/**
 * This selection method was implemented following pseudocode from
 *  "A Reinforcement Learning – Great-Deluge Hyper-heuristic for Examination Timetabling"
 *  (Özcan, Ender, et al.)
 **/
ReinforcementLearning::ReinforcementLearning(int utilityUpperBoundFactor, double initialUtilityFactor,
                                             const std::string& penaltyStrategy, const std::string& rewardStrategy)
        : Selector(), lastLLHUsed(), lowerUtilityBound(), utilityValues(operators.size()), ids(operators.size()) {

    //calculate upper bound of a utility value as
    upperUtilityBound = operators.size() * 5 * utilityUpperBoundFactor;

    //initialise utility values of each operator
    for (int i = 0; i < operators.size(); i++) {
        utilityValues[i] = upperUtilityBound * initialUtilityFactor;
    }

    initialisePenaltyStrategy(penaltyStrategy);
    initialiseRewardStrategy(rewardStrategy);
}

void ReinforcementLearning::select(boardType &board) {
    //identify operator(s) with maximal utility value(s)
    auto it = std::max_element(std::begin(utilityValues), std::end(utilityValues));
    while (it != std::end(utilityValues)) {
        ids.push_back(std::distance(std::begin(utilityValues), it));
        it = std::find(std::next(it), std::end(utilityValues), *it);
    }

    //randomly select operator with maximal utility
    applyOperator(board, ids[fastrand() % ids.size()]);

    //clear work vector for next iteration
    ids.clear();
}

void ReinforcementLearning::updateState(int objectiveChange) {
    if (objectiveChange < 0) {
        //reward operator if it improved objective
        utilityValues[lastLLHUsed] = std::min(reward_function(utilityValues[lastLLHUsed]), (double) upperUtilityBound);
    } else {
        //punish operator if it worsened objective
        utilityValues[lastLLHUsed] = std::max(penalty_function(utilityValues[lastLLHUsed]), (double) lowerUtilityBound);
    }
}

/**
* The negative reinforcement strategies used are the same as described in
*  "CHOOSING SEARCH HEURISTICS BY NONSTATIONARY REINFORCEMENT LEARNING"
*  (Nareyek, Alexander)
**/
void ReinforcementLearning::initialisePenaltyStrategy(const std::string& penaltyStrategy) {
    if(penaltyStrategy == "SUB" || penaltyStrategy == "SUBTRACTIVE" || penaltyStrategy == "P1"){
        penalty_function = [](double u){ return u - 1; };
    }
    else if(penaltyStrategy == "DIV" || penaltyStrategy == "DIVISIONAL" || penaltyStrategy == "P2"){
        penalty_function = [](double u){ return u / 2; };
    }
    else if(penaltyStrategy == "ROOT" || penaltyStrategy == "P3"){
        penalty_function = [](double u){ return sqrt(u); };
    } else {
        std::cout << "Invalid negative reinforcement strategy selected, choosing P2 (divisional) by default\n";
        penalty_function = [](double u){ return u / 2; };
    }
}

/**
* The positive reinforcement strategies used are the same as described in
*  "CHOOSING SEARCH HEURISTICS BY NONSTATIONARY REINFORCEMENT LEARNING"
*  (Nareyek, Alexander)
**/
void ReinforcementLearning::initialiseRewardStrategy(const std::string &rewardStrategy) {
    if(rewardStrategy == "ADD" || rewardStrategy == "ADDITIVE" || rewardStrategy == "R1"){
        reward_function = [](double u){ return u + 1; };
    }
    else if(rewardStrategy == "MUL" || rewardStrategy == "MULTIPLICATIVE" || rewardStrategy == "R2"){
        reward_function = [](double u){ return u * 2; };
    }
    else if(rewardStrategy == "POW" || rewardStrategy == "POWER" || rewardStrategy == "R3"){
        reward_function = [](double u){ return pow(u, 2); };
    } else {
        std::cout << "Invalid positive reinforcement strategy selected, choosing R2 (multiplicative) by default\n";
        reward_function = [](double u){ return u * 2; };
    }
}
