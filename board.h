#ifndef SUDOKU_OOP_BOARD_H
#define SUDOKU_OOP_BOARD_H

#include <string>
#include <cmath>
#include <vector>
#include <iostream>

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
    int totalObjective;

    MoveHistory moveHistory;

    void printBoard();

    //records a change from a sequence of moves
    void rememberChange(std::vector<MoveData> &changedCells);
    //restores board and objective values to last recorded state, discarding respective entry from history
    void undoChange();
    //discards all records of previous changes from history
    void acceptChange();

private:
    static std::string cellToString(int cell);
};

#endif //SUDOKU_OOP_BOARD_H
