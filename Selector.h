#ifndef SUDOKU_OOP_SELECTOR_H
#define SUDOKU_OOP_SELECTOR_H

#include <string>
#include <vector>
#include "Solver.h"
#include "board.h"

using std::string;
using std::vector;

enum selectionMethod{
    SR = 0,
    RD = 1,
    RP = 2,
    RPD = 3,
    GR = 4,

    LAST_METHOD = 5,
};

enum LLH{
    NEIGHBOURHOOD_SWAP = 0,
    NEIGHBOURHOOD_INSERT = 1,
    NEIGHBOURHOOD_INVERT = 2,
    NEIGHBOURHOOD_CPOExchange = 3,

    LAST_LLH = 2,
};

class Selector{
public:
    explicit Selector(boardType board, selectionMethod selectionMethod);
    int select();
private:
    boardType board;
    selectionMethod selectionMethod;
    vector<LLH> permutation;
    int currentLLHid;

    int simpleRandom();
    int randomDescent();
    int randomPermutation();
    int randomPermutationDescent();
    int greedySelection();

    int applyLLH(LLH selectedHeuristic);
};



#endif //SUDOKU_OOP_SELECTOR_H
