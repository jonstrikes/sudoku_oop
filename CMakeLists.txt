cmake_minimum_required(VERSION 3.17)
project(sudoku_oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-s -O3")

set(PROJECT_HEADERS
        Solver.h
        SudokuIO.h
        #utils
        utils/boardUtils.h
        #operators
        operators/operators.h
        #selectors
        models/selector.h
        selectors/simpleRandom.h
        #acceptors
        models/acceptor.h
        acceptors/improveOrEqual.h

        MoveHistory.h
        MoveData.h
        board.h)

set(PROJECT_SOURCES
        main.cpp
        SudokuIO.cpp
        #utils
        utils/boardUtils.cpp
        #operators
        operators/swap.cpp
        operators/insert.cpp
        operators/invert.cpp
        operators/cpoExchange.cpp
        #selectors
        models/selector.cpp
        selectors/simpleRandom.cpp

        #acceptors
        models/acceptor.cpp
        acceptors/improveOrEqual.cpp

        MoveHistory.cpp
        MoveData.cpp
        Solver.cpp
        board.cpp
        )

add_executable(sudoku_oop ${PROJECT_SOURCES} ${PROJECT_HEADERS})