#include <string>

#include "SudokuIO.h"
#include "models/selector.h"

#include <time.h>

#include "acceptors/improveOrEqual.h"
#include "acceptors/onlyImprove.h"
#include "acceptors/simulatedAnnealing.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/greedy.h"

int main() {
    std::string order3 = "benchmark_puzzles/benchmarks4x4/40/puzzle1.txt";
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
    RandomDescent selector;
      //OnlyImprove acceptor(board);
    SimulatedAnnealing acceptor(board, selector);

    int iterationCount = 0;
    clock_t tStart = clock();

    // isSolved or timout
    while(!acceptor.isSolved()){
        iterationCount++;
        //move selection
        selector.select(board);
        //move acceptance
        int change = acceptor.process(board);

        selector.updateState(change);
    }

    printf("Total iterations: %d\n", iterationCount );
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //print and thoroughly verify final solution
    board.printBoard();
    board.verifySolved();

    return 0;
}
