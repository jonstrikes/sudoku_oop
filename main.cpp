#include <string>

#include "SudokuIO.h"
#include "procedures/cpProcedure.h"
#include <ctime>

int main(int argc, char **argv) {
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

    cpProcedure(board, 1);
    board.printBoard();

    board.generateInitialSolution();
    board.printBoard();

    std::cout << board.calculateObjective() << "\n";

    for(int i=0; i<board.N; i++){
        for(int j=0; j<board.N; j++){
            std::cout << board.fixed[i][j] << " ";
        }
        std::cout << "\n";
    }

    //initialise specified hyper-heuristic
    Selector *selector;
    Acceptor *acceptor;
    readSelectorMethod(selectorMethod, selector);
    readAcceptorMethod(acceptorMethod, acceptor, selector, board);

    clock_t tStart = clock();
    double stime = 0, atime = 0;

    int iterationLimit = 1000000;
    int iterations = 0;
    // isSolved or timout
    while(!acceptor->isSolved()){
//    while(iterations < iterationLimit){
//        if(iterations > iterationLimit)
//            break;
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
    bool isSolved = board.verifySolved();

    double timeTaken = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    double iterationsPerSecond = iterations/((double)(clock() - tStart)/CLOCKS_PER_SEC);

    printf("\nTotal iterations:     %d\n", selector->getIterations());
    printf("Iterations per second:  %.2f\n", iterations/((double)(clock() - tStart)/CLOCKS_PER_SEC));
    printf("Time taken:             %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Sel and Acc time:       %.2fs\n", (stime + atime)/CLOCKS_PER_SEC);
    printf("Selector time taken:    %.5fs\n", stime/CLOCKS_PER_SEC);
    printf("Acceptor time taken:    %.5fs\n", atime/CLOCKS_PER_SEC);
    selector->printOperatorCounts();

    //to-file output
    std::string outputPath, fileName, runId;
    prepareOutput(puzzlePath, outputPath, fileName, runId, selectorMethod, acceptorMethod);

    writeSolution(board, outputPath, fileName, runId);
    writeGeneralLog(board, selector, acceptor, selectorMethod, acceptorMethod, fileName, isSolved, timeTaken, iterationsPerSecond);
    writeSelectorLog(board, selector, outputPath, fileName, runId);
    writeAcceptorLog(board, acceptor, outputPath, fileName, runId);

    //tidy up pointers
    delete acceptor;
    delete selector;
    acceptor = nullptr;
    selector = nullptr;

    return 0;
}
