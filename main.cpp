#include <string>
#include "SudokuIO.h"
#include "models/selector.h"

#include "acceptors/improveOrEqual.h"
#include "acceptors/onlyImprove.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/greedy.h"

int main() {
    std::string order3 = "benchmark_puzzles/benchmarks3x3/10/puzzle13.txt";
    std::string order4 = "benchmark_puzzles/benchmarks4x4/40/puzzle6.txt";
    std::string order5 = "benchmark_puzzles/benchmarks5x5/80/puzzle13.txt";

    srand(time(nullptr));

    //read and print problem
    boardType board = readFile(order3);
    board.printBoard();

    //fill in and print initial solution
    board.generateInitialSolution();
    board.printBoard();

    //initialise specified hyper-heuristic
    Greedy selector;
    OnlyImprove acceptor(board);

    // isSolved or timout
    while(!acceptor.isSolved()){
        //move selection
        selector.select(board);
        //move acceptance
        int change = acceptor.process(board);

        selector.updateState(change);
    }

    //print and thoroughly verify final solution
    board.printBoard();
    board.verifySolved();

    return 0;
}
