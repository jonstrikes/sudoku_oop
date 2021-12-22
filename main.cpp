#include <string>
#include "SudokuIO.h"
#include "Solver.h"
#include "LLH.h"
#include "Selector.h"

int main() {
    std::string order3 = "benchmark_puzzles/benchmarks3x3/40/puzzle19.txt";
    std::string order4 = "benchmark_puzzles/benchmarks4x4/40/puzzle6.txt";
    std::string order5 = "benchmark_puzzles/benchmarks5x5/80/puzzle13.txt";

    srand(time(nullptr));

    boardType board = readFile(order3);
    board.printBoard();

    fillGrid(&board);

    int colObjs[board.N];
    int rowObjs[board.N];

    //work vectors
    //previous cells as a tuple of <row, col, value>
    auto *cellsPrev = new std::vector<std::tuple<int, int, int>>();
    //previous objective values columns and rows that changed as a result of
    auto *rowPrevObjs = new std::map<int, int>();
    auto *colPrevObjs = new std::map<int, int>();

    int obj = calcObj(&board, rowObjs, colObjs);
    std::cout<<std::endl<< obj <<std::endl;
    board.printBoard();

    std::cout<<std::endl<< "NOW SWAPPING" <<std::endl;
    neighbourhoodSwap(&board, cellsPrev);
    board.printBoard();

    int gap = recalcObj(&board, rowObjs, colObjs, cellsPrev, rowPrevObjs, colPrevObjs);
    std::cout<<std::endl<< "CHANGE CALCULATED: " << gap << " CURRENT OBJECTIVE VALUE: " << calcObj(&board, rowObjs, colObjs) <<std::endl;
    board.printBoard();

    undoMove(&board, rowObjs, colObjs, cellsPrev, rowPrevObjs, colPrevObjs);
    std::cout<<std::endl<< "UNDONE MOVE:" << " CURRENT OBJECTIVE VALUE: " << calcObj(&board, rowObjs, colObjs) <<  std::endl;
    board.printBoard();

//    int change;
//    auto selector = Selector(selectionMethod(SR));
//    //test one run
//
//    //selection
//    change = selector.select();
//
//    // move acceptance

    return 0;
}
