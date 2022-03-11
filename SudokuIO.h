#ifndef SUDOKU_OOP_SUDOKUIO_H
#define SUDOKU_OOP_SUDOKUIO_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include "board.h"

#include "models/selector.h"
#include "acceptors/improveOrEqual.h"
#include "acceptors/onlyImprove.h"
#include "acceptors/simulatedAnnealing.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/greedy.h"


boardType readFile(const std::string& fileDir);
bool readCMDParams(char **input, int size, std::string& puzzleDir, std::string& outputLog, std::string& solutionOutput);
bool readCMDOptionalParams(char **input, int size, std::string& acceptorType, std::string& selectorType);
void readAcceptorMethod(const std::string& acceptorMethod, Acceptor *&acceptor, Selector *&selector, boardType &board);
void readSelectorMethod(const std::string& selectorMethod, Selector *&selector);

#endif //SUDOKU_OOP_SUDOKUIO_H
