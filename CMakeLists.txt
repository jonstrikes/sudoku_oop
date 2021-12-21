cmake_minimum_required(VERSION 3.17)
project(sudoku_oop)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_HEADERS
        Solver.h
        SudokuIO.h
        LLH.h
        board.h)

set(PROJECT_SOURCES
        main.cpp
        SudokuIO.cpp
        LLH.cpp
        Solver.cpp
        board.cpp
        )

add_executable(sudoku_oop ${PROJECT_SOURCES} ${PROJECT_HEADERS})