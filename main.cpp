#include <string>
#include <ctime>

#include "SudokuIO.h"

int main(int argc, char **argv) {
    srand(time(nullptr));

    std::string puzzlePath;
    std::string acceptorMethod, selectorMethod;

    if (!readCMDParams(argv, argc, puzzlePath)) {
        //user input could not be parsed
        return 0;
    } else {
        //see if there are acceptor and selector methods supplied
        readCMDOptionalParams(argv, argc, acceptorMethod, selectorMethod);
    }

    //parse board from input
    boardType board = readFile(puzzlePath);
    board.printBoard();

    board.fixBlocksWithSingleCellMissing();
    vector<vector<bool>> originalFixedMap(board.fixed);

    //try to fill cell implicitly
    cpProcedure(board, 1.0);
    //fill remaining empty cells
    board.generateSolution();

    auto specs = readSpecification();

    //initialise CMD specified hyper-heuristic with parameters specified in specs.json and readme
    Selector *selector;
    Acceptor *acceptor;
    readSelectorMethod(selectorMethod, specs, selector);
    readAcceptorMethod(acceptorMethod, specs, acceptor, selector, board);

    //initialise constraint programming procedure with parameters specified in specs.json and readme
    CpProcessor *cpProcessor;
    readCpParams(board, acceptor, specs, cpProcessor);

    //read any other params
    double timeLimit;
    readGeneralParams(board, specs, timeLimit);

    //start of algorithm
    bool timeOut = false;
    clock_t tStart = clock();
    clock_t tFinish = tStart + (timeLimit*CLOCKS_PER_SEC);
    double stime = 0, atime = 0;

    int iterationLimit = 3000000;
    int iterations = 0;

    // isSolved or timout
    while (!acceptor->isSolved()) {
    //while(iterations <= iterationLimit){

        //check if we still have time
        if(clock() >= tFinish){
            timeOut = true;
            break;
        }

        iterations++;

        //move selection
        clock_t sStart = clock();
        selector->select(board);
        stime += (double) (clock() - sStart);

        //move acceptance
        clock_t aStart = clock();
        int change = acceptor->process(board);
        atime += (double) (clock() - aStart);

        selector->updateState(change);

        cpProcessor->run();
    }

    double timeTaken = (double) (clock() - tStart) / CLOCKS_PER_SEC;
    double iterationsPerSecond = iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC);

    printf("\nTotal iterations:     %d\n", selector->getIterations());
    printf("Iterations per second:  %.2f\n", iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC));
    printf("Time taken:             %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
    printf("Sel and Acc time:       %.2fs\n", (stime + atime) / CLOCKS_PER_SEC);
    printf("Selector time taken:    %.5fs\n", stime / CLOCKS_PER_SEC);
    printf("Acceptor time taken:    %.5fs\n", atime / CLOCKS_PER_SEC);
    selector->printOperatorCounts();

    //print and thoroughly verify final solution
    board.printBoard();
    bool isSolved = board.verifySolved();

    //to-file output
    std::string outputPath, fileName, runId;
    prepareOutput(puzzlePath, outputPath, fileName, runId, selectorMethod, acceptorMethod);

    writeSolution(board, outputPath, fileName, runId);
    writeGeneralLog(board, selector, acceptor, selectorMethod, acceptorMethod, fileName, isSolved, timeTaken,
                    iterationsPerSecond);
    writeSelectorLog(board, selector, outputPath, fileName, runId);
    writeAcceptorLog(board, acceptor, outputPath, fileName, runId);

    //tidy up pointers
    delete acceptor;
    delete selector;
    acceptor = nullptr;
    selector = nullptr;

    return 0;
}
