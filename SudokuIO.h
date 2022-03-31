#ifndef SUDOKU_OOP_SUDOKUIO_H
#define SUDOKU_OOP_SUDOKUIO_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <filesystem>
#include <string>

#include "board.h"

#include "models/selector.h"
#include "acceptors/improveOrEqual.h"
#include "acceptors/onlyImprove.h"
#include "acceptors/simulatedAnnealing.h"
#include "acceptors/adaptiveIterationLimitedThresholdAccepting.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/reinforcementLearning.h"

//input functions
boardType readFile(const std::string &fileDir);

bool readCMDParams(char **input, int size, std::string &puzzleDir, std::string &outputLog, std::string &solutionOutput);

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType);

void readAcceptorMethod(const std::string &acceptorMethod, Acceptor *&acceptor, Selector *&selector, boardType &board);

void readSelectorMethod(const std::string &selectorMethod, Selector *&selector);

//output functions
void prepareOutput(const std::string &inputPath, std::string &outputPath, std::string &fileName, std::string &runId,
                   std::string selectorMethod, std::string acceptorMethod);

void writeSolution(boardType &board, const std::string &outputPath, const std::string &fileName,
                   const std::string &runId);

void writeSelectorLog(boardType &board, Selector *&selector, const std::string &outputPath, const std::string &fileName,
                      const std::string &runId);

void writeAcceptorLog(boardType &board, Acceptor *&acceptor, const std::string &outputPath, const std::string &fileName,
                      const std::string &runId);

void writeGeneralLog(boardType &board, Selector *&selector, Acceptor *&acceptor, std::string selectorMethod,
                     std::string acceptorMethod, std::string fileName, bool isSolved, double timeTaken, double iterationsPerSecond);

#endif //SUDOKU_OOP_SUDOKUIO_H
