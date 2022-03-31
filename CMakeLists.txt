cmake_minimum_required(VERSION 3.17)
project(sudoku_oop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-s -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(PROJECT_HEADERS
        SudokuIO.h
        #utils
        utils/boardUtils.h
        utils/generalUtils.h

        #operators
        operators/operators.h

        #selectors
        models/selector.h
        selectors/simpleRandom.h
        selectors/randomDescent.h
        selectors/randomPermutation.h
        selectors/randomPermutationDescent.h
        selectors/reinforcementLearning.h

        #acceptors
        models/acceptor.h
        acceptors/improveOrEqual.h
        acceptors/onlyImprove.h
        acceptors/simulatedAnnealing.h
        acceptors/adaptiveIterationLimitedThresholdAccepting.h

        #procedures
        procedures/cpProcedure.h

        MoveRecord.h
        board.h)

set(PROJECT_SOURCES
        main.cpp
        SudokuIO.cpp
        #utils
        utils/boardUtils.cpp
        utils/generalUtils.cpp

        #operators
        operators/swap.cpp
        operators/insert.cpp
        operators/invert.cpp
        operators/cpoExchange.cpp
        operators/insertSym.cpp
        operators/invertSym.cpp
        operators/swapConflicting.cpp

        #selectors
        models/selector.cpp
        selectors/simpleRandom.cpp
        selectors/randomDescent.cpp
        selectors/randomPermutation.cpp
        selectors/randomPermutationDescent.cpp
        selectors/reinforcementLearning.cpp

        #acceptors
        models/acceptor.cpp
        acceptors/improveOrEqual.cpp
        acceptors/onlyImprove.cpp
        acceptors/simulatedAnnealing.cpp
        acceptors/adaptiveIterationLimitedThresholdAccepting.cpp

        #procedures
        procedures/cpProcedure.cpp

        MoveRecord.cpp
        board.cpp)

add_executable(sudoku_oop ${PROJECT_SOURCES} ${PROJECT_HEADERS})

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(sudoku_oop PRIVATE nlohmann_json::nlohmann_json)