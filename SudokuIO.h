#ifndef SUDOKU_OOP_SUDOKUIO_H
#define SUDOKU_OOP_SUDOKUIO_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include "board.h"


boardType readFile(const std::string& fileDir);

#endif //SUDOKU_OOP_SUDOKUIO_H
