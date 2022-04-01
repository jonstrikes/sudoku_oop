#ifndef SUDOKU_OOP_REINFORCEMENTLEARNING_H
#define SUDOKU_OOP_REINFORCEMENTLEARNING_H

#include "../models/selector.h"


/**
 * This selection method was implemented following pseudocode from
 *  "A Reinforcement Learning – Great-Deluge Hyper-heuristic for Examination Timetabling"
 *  (Özcan, Ender, et al.)
 **/
class ReinforcementLearning : public Selector {
private:
    vector<double> utilityValues;
    std::vector<int> ids;

    int lastLLHUsed;
    int lowerUtilityBound;
    int upperUtilityBound;

    std::function<double(double)> penalty_function;
    std::function<double(double)> reward_function;

    /**
     * The negative reinforcement strategies used are the same as described in
     *  "CHOOSING SEARCH HEURISTICS BY NONSTATIONARY REINFORCEMENT LEARNING"
     *  (Nareyek, Alexander)
    **/
    void initialisePenaltyStrategy(const std::string &penaltyStrategy);

    /**
     * The positive reinforcement strategies used are the same as described in
     *  "CHOOSING SEARCH HEURISTICS BY NONSTATIONARY REINFORCEMENT LEARNING"
     *  (Nareyek, Alexander)
    **/
    void initialiseRewardStrategy(const std::string &rewardStrategy);

public:
    explicit ReinforcementLearning(int utilityUpperBoundFactor, double initialUtilityFactor,
                                   const std::string &penaltyStrategy, const std::string &rewardStrategy);

    void select(boardType &board) override;

    void updateState(int change) override;
};


#endif //SUDOKU_OOP_REINFORCEMENTLEARNING_H
