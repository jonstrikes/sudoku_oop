#ifndef SUDOKU_OOP_BOARD_H
#define SUDOKU_OOP_BOARD_H

#include <string>
#include <cmath>
#include <vector>
#include <iostream>

using std::vector;

struct boardType{
    //CONSIDER ADDING A SCORE HERE INSTEAD OF USING THAT CRINGE CHANGE VAR
    int n;
    int N;
    int minCellValue;
    int maxCellValue;
    vector<vector<int>> board;
    vector<vector<int>> fixed;

    vector<int> colObjs;
    vector<int> rowObjs;

    void printBoard();

private:
    static std::string cellToString(int cell);
};

#endif //SUDOKU_OOP_BOARD_H
