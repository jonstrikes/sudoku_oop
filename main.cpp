#include <string>
#include <ctime>

#include "SudokuIO.h"

int main(int argc, char **argv) {
    srand(time(nullptr));

    std::string puzzlePath, solutionOutPath, generalLogOutPath, accLogOutPath, selLogOutPath;
    std::string acceptorMethod, selectorMethod;

    if (!readCMDParams(argv, argc, puzzlePath, solutionOutPath, generalLogOutPath, accLogOutPath, selLogOutPath)) {
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
    clock_t tStart = clock();
    clock_t tFinish = tStart + (timeLimit * CLOCKS_PER_SEC);
    double stime = 0, atime = 0;

    double LOG_ITERATIONS_FACTOR = specs["General"]["LOG_ITERATIONS_FACTOR"];
    int unfixedCount = board.countUnfixedCells();
    int iterationLimit = round(pow(unfixedCount, 2)) * LOG_ITERATIONS_FACTOR;


    int iterations = 0;
    while (!acceptor->isSolved()) {
        //check if we still have time
        if (clock() >= tFinish) {
            break;
        }

        iterations++;
        if(iterations % iterationLimit == 0){
            acceptor->printShortLog();
        }

        //move selection
        clock_t sStart = clock();
        selector->select(board);
        stime += (double) (clock() - sStart);

        //move acceptance
        clock_t aStart = clock();
        int change = acceptor->process(board);
        atime += (double) (clock() - aStart);

        selector->updateState(change);

        //runs every
        cpProcessor->run();
    }

    double timeTaken = (double) (clock() - tStart) / CLOCKS_PER_SEC;
    double iterationsPerSecond = iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC);

    selector->printLog();
    acceptor->printLog();

    printf("\n=========== Summary  ===========\n");
    printf("%-30.30s %d\n", "Total iterations:", selector->getIterations());
    printf("%-30.30s %.2fs\n", "Iterations per second:", iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC));
    printf("%-30.30s %.2fs\n", "Time taken:", (double) (clock() - tStart) / CLOCKS_PER_SEC);
    printf("%-30.30s %.2fs\n", "Sel and Acc time:", (stime + atime) / CLOCKS_PER_SEC);
    printf("%-30.30s %.2fs\n", "Selector time taken:", stime / CLOCKS_PER_SEC);
    printf("%-30.30s %.2fs\n\n", "Acceptor time taken:", atime / CLOCKS_PER_SEC);

    //print and thoroughly verify final solution
    board.printBoard();
    bool isSolved = board.verifySolved();

    //to-file output
    writeSolution(board, solutionOutPath);
    writeGeneralLog(board, selector, acceptor, selectorMethod, acceptorMethod,
                    generalLogOutPath, solutionOutPath, isSolved, timeTaken, iterationsPerSecond);
    writeSelectorLog(board, selector, selLogOutPath, solutionOutPath);
    writeAcceptorLog(board, acceptor, accLogOutPath, solutionOutPath);

    //tidy up pointers
    delete acceptor;  delete selector; delete cpProcessor;
    acceptor = nullptr; selector = nullptr; cpProcessor = nullptr;
    return 0;
}
