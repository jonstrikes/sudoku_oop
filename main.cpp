#include <string>
#include "SudokuIO.h"
#include "Solver.h"
#include "models/selector.h"
#include "acceptors/improveOrEqual.h"
#include "selectors/simpleRandom.h"

int main() {
    std::string order3 = "benchmark_puzzles/benchmarks3x3/80/puzzle3.txt";
    std::string order4 = "benchmark_puzzles/benchmarks4x4/40/puzzle6.txt";
    std::string order5 = "benchmark_puzzles/benchmarks5x5/80/puzzle13.txt";

    srand(time(nullptr));

    boardType board = readFile(order3);
    board.printBoard();
    fillGrid(&board);
    board.printBoard();

//    int iterations = 0;
//
//    int obj = calcObj(&board);
//    while(obj != 0){
//        cpoExchange(board);
//        int objChange = recalcObj(&board);
//
//        if(objChange > 0) {
//            board.undoChange();
//        }
//        else {
//            obj += objChange;
//            board.acceptChange();
//        }
//
//        iterations++;
//        std::cout << std::endl << objChange << "estimated obj is " << obj <<  " AND ACTUAL OBJECTIVE IS " <<  calcObj(&board) << std::endl;
//    }
//
//    board.printBoard();
//    std::cout << std::endl << calcObj(&board) << " Num of iteration: " << iterations << std::endl;



//    std::cout<<std::endl<< calcObj(&board) <<std::endl;
//    board.printBoard();
//    std::cout<<std::endl<< "NOW EXCHANGING" <<std::endl;
//    neighbourhoodCPOEx(board);
//
//    int gap = recalcObj(&board);
//    std::cout<<std::endl<< "CHANGE CALCULATED: " << gap << " CURRENT OBJECTIVE VALUE: " << calcObj(&board) <<std::endl;
//    board.printBoard();
//
//    board.undoChange();
//    std::cout<<std::endl<< "UNDONE MOVE:" << " CURRENT OBJECTIVE VALUE: " << calcObj(&board) <<  std::endl;
//    board.printBoard();

//    int gap = recalcObj(&board);
//    std::cout<<std::endl<< "CHANGE CALCULATED: " << gap << " CURRENT OBJECTIVE VALUE: " << calcObj(&board) <<std::endl;

//    std::cout<<std::endl<< calcObj(&board) <<std::endl;
//    board.printBoard();

    //test one run
    SimpleRandom selector;
    ImproveOrEqual acceptor(board);

    //move selection
    selector.select(board);
    board.printBoard();

    //move acceptance
    acceptor.process(board);
    board.printBoard();

    while(!acceptor.isSolved()){ // isSolved or timout
        //move selection
        selector.select(board);
        //move acceptance
        acceptor.process(board);
    }

    board.printBoard();


    return 0;
}
