#include <string>
#include "SudokuIO.h"
#include "models/selector.h"

#include "acceptors/improveOrEqual.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/greedy.h"

int main() {
    std::string order3 = "benchmark_puzzles/benchmarks3x3/80/puzzle3.txt";
    std::string order4 = "benchmark_puzzles/benchmarks4x4/40/puzzle6.txt";
    std::string order5 = "benchmark_puzzles/benchmarks5x5/80/puzzle13.txt";

    srand(time(nullptr));

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

    //read and print problem
    boardType board = readFile(order3);
    board.printBoard();

    //fill in and print initial solution
    board.generateInitialSolution();
    board.printBoard();

    //initialise specified hyperheuristic
    Greedy selector;
    ImproveOrEqual acceptor(board);

    // isSolved or timout
    while(!acceptor.isSolved()){
        //move selection
        selector.select(board);
        //move acceptance
        int change = acceptor.process(board);

        selector.updateState(change);
    }

    board.printBoard();


    //check rows and cols of solution
    for(const std::vector<int>& row : board.board){
        std::vector<bool> encountered(board.N);

        for(int val : row){
            if(encountered[val]) {std::cout<< "! Duplicate row value found: " << val << std::endl;}
            encountered[val] = true;
        }

        //reset work vector
        encountered.assign(encountered.size(), false);
    }

    for(int i=0; i<board.N; i++){
        std::vector<bool> encountered(board.N);
        for(int j=0; j<board.N; j++){
            if(encountered[board.board[j][i]]) {std::cout<< "! Duplicate col value found: " << board.board[j][i] << std::endl;}
            encountered[board.board[j][i]] = true;
        }

        //reset work vector
        encountered.assign(encountered.size(), false);
    }

    return 0;
}
