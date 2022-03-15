#ifndef SUDOKU_OOP_RANDOMPERMUTATION_H
#define SUDOKU_OOP_RANDOMPERMUTATION_H

#include "../models/selector.h"
#include <random>
#include <algorithm>

class RandomPermutation : public Selector {
private:
    vector<uint_fast8_t> permutation;
    uint_fast8_t currentId;
public:
    explicit RandomPermutation();
    void select(boardType &board) override;
    void updateState(int change) override;
};

#endif //SUDOKU_OOP_RANDOMPERMUTATION_H
