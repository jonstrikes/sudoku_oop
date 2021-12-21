#ifndef SUDOKU_OOP_SOLVER_H
#define SUDOKU_OOP_SOLVER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <random>

#include <set>
#include <map>
#include <tuple>

#include "board.h"
#include "SudokuIO.h"

void fillGrid(boardType board);
void undoMove(boardType board, int *rowObjectives, int *colObjectives,
              std::vector<std::tuple<int, int, int>> *changedCells,
              std::map<int, int> *rowPrevObjs, std::map<int, int> *colPrevObjs);
int recalcObj(boardType board, int *rowObjectives, int *colObjectives,
              std::vector<std::tuple<int, int, int>> *changedCells,
              std::map<int, int> *rowPrevObjs, std::map<int, int> *colPrevObjs);
int calcObj(boardType board, int *rowObjectives, int *colObjectives);


#endif //SUDOKU_OOP_SOLVER_H
