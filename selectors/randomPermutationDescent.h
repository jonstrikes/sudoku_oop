#ifndef SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H
#define SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H

#include "../models/selector.h"
#include <random>
#include <algorithm>

class RandomPermutationDescent : public Selector {
private:
    vector<int> permutation;
    int currentId;
public:
    explicit RandomPermutationDescent();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H
