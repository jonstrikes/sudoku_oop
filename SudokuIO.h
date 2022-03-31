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

#include "nlohmann/json.hpp"

//input functions
boardType readFile(const std::string &fileDir);

nlohmann::json readSpecification();

bool readCMDParams(char **input, int size, std::string &puzzleDir);

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType);

void readAcceptorMethod(const std::string &acceptorMethod, Acceptor *&acceptor, Selector *&selector, boardType &board);

void readSelectorMethod(const std::string &selectorMethod, nlohmann::json &specs, Selector *&selector);


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
