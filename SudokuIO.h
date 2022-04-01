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

#include "procedures/cpProcedure.h"

#include "nlohmann/json.hpp"

//input functions
boardType readFile(const std::string &fileDir);

nlohmann::json readSpecification();

bool readCMDParams(char **input, int size, std::string &puzzleDir, std::string &solutionOutPath,
                   std::string &generalLogOutPath, std::string &accLogOutPath, std::string &selLogOutPath);

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType);

void readAcceptorMethod(const std::string &acceptorMethod, nlohmann::json &specs, Acceptor *&acceptor,
                        Selector *&selector, boardType &board);

void readSelectorMethod(const std::string &selectorMethod, nlohmann::json &specs, Selector *&selector);

void readCpParams(boardType &board, Acceptor *&acceptor, nlohmann::json &specs, CpProcessor *&CpProcessor);

void readGeneralParams(boardType &board, nlohmann::json specs, double &timeLimit);


//output functions
void writeSolution(boardType &board, const std::string &outputFile);

void writeSelectorLog(boardType &board, Selector *&selector, const std::string &selLogOutPath,
                      const std::string &solutionOutPath);

void writeAcceptorLog(boardType &board, Acceptor *&acceptor, const std::string &accLogOutPath,
                      const std::string &solutionOutPath);

void writeGeneralLog(boardType &board, Selector *&selector, Acceptor *&acceptor, std::string selectorMethod,
                     std::string acceptorMethod, const std::string& generalLogPath, const std::string& solutionOutPath,
                     bool isSolved, double timeTaken, double iterationsPerSecond);

#endif //SUDOKU_OOP_SUDOKUIO_H
