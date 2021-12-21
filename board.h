#ifndef SUDOKU_OOP_BOARD_H
#define SUDOKU_OOP_BOARD_H

#include <string>
#include <cmath>
#include <iostream>

struct boardType{
    int n;
    int N;
    int minCellValue;
    int maxCellValue;
    int **board;
    int **fixed;
};

void printBoard(boardType);
std::string cellToString(int cell);

#endif //SUDOKU_OOP_BOARD_H
