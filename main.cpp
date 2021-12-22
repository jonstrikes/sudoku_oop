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

    int obj = calcObj(&board);
    std::cout<<std::endl<< obj <<std::endl;
    board.printBoard();

    std::cout<<std::endl<< "NOW SWAPPING" <<std::endl;
    neighbourhoodSwap(&board);

    int gap = recalcObj(&board);
    std::cout<<std::endl<< "CHANGE CALCULATED: " << gap << " CURRENT OBJECTIVE VALUE: " << calcObj(&board) <<std::endl;
    board.printBoard();

    board.undoChange();
    std::cout<<std::endl<< "UNDONE MOVE:" << " CURRENT OBJECTIVE VALUE: " << calcObj(&board) <<  std::endl;
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
