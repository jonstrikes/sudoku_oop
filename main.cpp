#include <string>

#include "SudokuIO.h"
#include "models/selector.h"

#include <time.h>
#include <filesystem>

#include "acceptors/improveOrEqual.h"
#include "acceptors/onlyImprove.h"
#include "acceptors/simulatedAnnealing.h"

#include "selectors/simpleRandom.h"
#include "selectors/randomDescent.h"
#include "selectors/randomPermutation.h"
#include "selectors/randomPermutationDescent.h"
#include "selectors/greedy.h"

int main(int argc, char **argv) {
    std::string order3 = "benchmark_puzzles/benchmarks4x4/40/puzzle1.txt";
    std::string order4 = "benchmark_puzzles/benchmarks4x4/40/puzzle6.txt";
    std::string order5 = "benchmark_puzzles/benchmarks5x5/80/puzzle13.txt";

    srand(time(nullptr));

    std::string puzzlePath, outputLogFile, solutionFile;
    std::string acceptorMethod, selectorMethod;

    if(!readCMDParams(argv, argc, puzzlePath, outputLogFile, solutionFile)){
        //user input could not be parsed
        return 0;
    }
    else{
        //see if there are acceptor and selector methods supplied
        readCMDOptionalParams(argv, argc, acceptorMethod, selectorMethod);
    }


    //scan board and generate initial guess solution
    boardType board = readFile(puzzlePath);
    board.printBoard();
    board.generateInitialSolution();

    //initialise specified hyper-heuristic
    Selector *selector;
    Acceptor *acceptor;

    readSelectorMethod(selectorMethod, selector);
    readAcceptorMethod(acceptorMethod, acceptor, selector, board);

    clock_t tStart = clock();

    double stime = 0;
    double atime = 0;

    int iterationLimit = 1000000;
    int iterations = 0;
    // isSolved or timout
    while(!acceptor->isSolved()){
        if(iterations > iterationLimit)
            break;
        iterations++;
        //move selection
            clock_t sStart = clock();
        selector->select(board);
            stime += (double)(clock() - sStart);

        //move acceptance
            clock_t aStart = clock();
        int change = acceptor->process(board);
            atime += (double)(clock() - aStart);

        selector->updateState(change);
    }

    //print and thoroughly verify final solution
    board.printBoard();
    board.verifySolved();

    printf("Total iterations: %d\n", selector->getIterations());
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Time taken: %.2fs\n", (stime + atime)/CLOCKS_PER_SEC);
    printf("Selector time taken: %.5fs\n", stime/CLOCKS_PER_SEC);
    printf("Acceptor time taken: %.5fs\n", atime/CLOCKS_PER_SEC);
    selector->printOperatorCounts();

    delete acceptor;
    delete selector;
    acceptor = nullptr;
    selector = nullptr;

    return 0;
}
