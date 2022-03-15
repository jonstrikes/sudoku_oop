#ifndef SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H
#define SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H

#include "../models/selector.h"
#include <random>
#include <algorithm>

class RandomPermutationDescent : public Selector {
private:
    vector<uint_fast8_t> permutation;
    uint_fast8_t currentId;
public:
    explicit RandomPermutationDescent();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_RANDOMPERMUTATIONDESCENT_H
