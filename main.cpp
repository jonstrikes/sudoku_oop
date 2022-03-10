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

    printf("%d\n", argc);
    if(argc != 4 && argc != 6){
        printf("Sudoku Solver\n");
        printf("Usage: PuzzlePath OutputFile SolutionFile -acceptor -selector\n");
        printf("If either the acceptor or selector is not specified, the program defaults to Simple Random and Improve or Equal\n");
        printf("Acceptors: -oi, -ie, -sa, -gd, -la\n");
        printf("Selectors: -sr, -rd, -rp, -rpd, -g\n");
        return 0;
    }

    std::string puzzlePath;
    std::string outputLogFile;
    std::string solutionFile;

    std::string acceptorMethod;
    std::string selectorMethod;

    puzzlePath = argv[1];
    outputLogFile = argv[2];
    solutionFile = argv[3];

    if(argc == 6){
        acceptorMethod = argv[4];
        selectorMethod = argv[5];
    }

    //scan board and generate initial guess solution
    boardType board = readFile(puzzlePath);
    board.printBoard();
    board.generateInitialSolution();

    //initialise specified hyper-heuristic
    Selector *selector;
    if(selectorMethod == "--simple-random" || selectorMethod == "-sr"){
        selector = new SimpleRandom();
    }
    else if(selectorMethod == "--random-descent" || selectorMethod == "-rd"){
        selector = new RandomDescent();
    }
    else if(selectorMethod == "--random-permutation" || selectorMethod == "-rp"){
        selector = new RandomPermutation();
    }
    else if(selectorMethod == "--random-permutation-descent" || selectorMethod == "-rpd"){
        selector = new RandomPermutationDescent();
    }
    else if(selectorMethod == "--greedy" || selectorMethod == "-g"){
        selector = new Greedy();
    }
    else{
        selector = new SimpleRandom();
        printf("Selector %s not found\n", selectorMethod.c_str());
    }

    Acceptor *acceptor;
    if(acceptorMethod == "--only-improve" || acceptorMethod == "-oi"){
        acceptor = new OnlyImprove(board);
    }
    else if(acceptorMethod == "--improve-or-equal" || acceptorMethod == "-ie"){
        acceptor = new ImproveOrEqual(board);
    }
    else if(acceptorMethod == "--simulated-annealing" || acceptorMethod == "-sa"){
        acceptor = new SimulatedAnnealing(board, *selector);
    }
//    else if(acceptorMethod == "--great-deluge" || acceptorMethod == "-gd"){
//        acceptor = new GreatDeluge(board);
//    }
//    else if(acceptorMethod == "--late-acceptance" || acceptorMethod == "-la"){
//        acceptor = new LateAcceptance(board);
//    }
    else{
        acceptor = new ImproveOrEqual(board);
        printf("Acceptor %s not found\n", acceptorMethod.c_str());
    }

    int iterationCount = 0;
    clock_t tStart = clock();

    // isSolved or timout
    while(!acceptor->isSolved()){
        iterationCount++;
        //move selection
        selector->select(board);
        //move acceptance
        int change = acceptor->process(board);

        selector->updateState(change);
    }

    printf("Total iterations: %d\n", iterationCount );
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //print and thoroughly verify final solution
    board.printBoard();
    board.verifySolved();


    delete acceptor;
    delete selector;
    acceptor = nullptr;
    selector = nullptr;

    return 0;
}
