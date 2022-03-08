#ifndef SUDOKU_OOP_BOARD_H
#define SUDOKU_OOP_BOARD_H

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "MoveHistory.h"

using std::vector;

struct boardType{
    //CONSIDER ADDING A SCORE HERE INSTEAD OF USING THAT CRINGE CHANGE VAR
    int n;
    int N;
    int minCellValue;
    int maxCellValue;

    vector<vector<int>> board;
    vector<vector<int>> fixed;

    vector<int> rowObjectives;
    vector<int> colObjectives;

    MoveHistory moveHistory;

    void printBoard();

    //checks if there are moves recorded in history
    bool hasChange();
    //records a change from a sequence of moves
    void rememberChange(std::vector<MoveData> &changedCells);
    //restores board and objective values to last recorded state, discarding respective entry from history
    void undoChange();
    //discards all records of previous changes from history
    void acceptChange();
    //generates an initial solution or randomly reassigns an existing one
    void generateInitialSolution();
    //assigns row and column objective scores for the entire board
    int calculateObjective();
    //updates row and column objectives based on the last change recorded
    int updateObjective();

private:
    static std::string cellToString(int cell);
};

#endif //SUDOKU_OOP_BOARD_H
