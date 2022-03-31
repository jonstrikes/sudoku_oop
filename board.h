#ifndef SUDOKU_OOP_BOARD_H
#define SUDOKU_OOP_BOARD_H

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <set>

#include "MoveRecord.h"

using std::vector;

struct boardType {
    int n;
    int N;
    int minCellValue;
    int maxCellValue;

    //2d vector representing the cell values of the board
    vector<vector<int>> board;
    //2d vector marking cell values that can not be changed
    vector<vector<bool>> fixed;

    vector<uint_fast8_t> rowObjectives;
    vector<uint_fast8_t> colObjectives;

    //previous state representation
    MoveRecord moveRecord;

    boardType(int n, int N, int minCellValue, int maxCellValue,
              vector<vector<int>> board, vector<vector<bool>> fixed);

    //displays the board to standard output and returns the string representation of the board
    std::string printBoard();

    //checks if there are moves recorded in history
    bool hasChange();

    //records a change from a sequence of moves
    void rememberChange(std::vector<MoveData> &changedCells);

    //restores board and objective values to last recorded state, discarding respective entry from history
    void undoChange();

    //discards all records of previous changes from history
    void acceptChange();

    //generates an initial solution or randomly reassigns an existing one
    void generateSolution();

    void fixBlocksWithSingleCellMissing();

    //shuffles the unfixed cells of the current solution
    void randomiseExistingSolution();

    //assigns row and column objective scores for the entire board
    int calculateObjective();

    //updates row and column objectives based on the last change recorded
    int updateObjective();

    //verifies if the board is a correct solution
    bool verifySolved();

    //returns coordinates of cells that are conflicting in row/col/block
    std::set<std::pair<uint_fast8_t, uint_fast8_t>> getConflictingCells();

private:
    //work vector used internally to avoid reinitialising
    vector<bool> encounteredAlongRow;
    //work vector used internally to avoid reinitialising
    vector<bool> encounteredAlongCol;
    //starting board, only used for thorough answer verification
    vector<vector<int>> startingBoard;
    //starting fixed cell map, only used for thorough answer verification
    vector<vector<bool>> startingFixed;

    static std::string cellToString(int cell);

    //private constructor and comparator to avoid implicit copying of board
    boardType(const boardType &);
    boardType &operator=(const boardType &);
};

#endif //SUDOKU_OOP_BOARD_H
