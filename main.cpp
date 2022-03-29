#include <string>

#include "SudokuIO.h"
#include "procedures/cpProcedure.h"
#include <ctime>

int main(int argc, char **argv) {
    srand(time(nullptr));

    std::string puzzlePath, outputLogFile, solutionFile;
    std::string acceptorMethod, selectorMethod;

    if (!readCMDParams(argv, argc, puzzlePath, outputLogFile, solutionFile)) {
        //user input could not be parsed
        return 0;
    } else {
        //see if there are acceptor and selector methods supplied
        readCMDOptionalParams(argv, argc, acceptorMethod, selectorMethod);
    }


    //scan board and generate initial guess solution
    boardType board = readFile(puzzlePath);
    board.printBoard();

    float resetFactor = 1.0;
    float resetAlpha = 0.5;
    float resetBeta = 2;

    int worseningCyclesLimit = 50;
    int worseningCycles = 0;

    int cpIterationLimit = board.N * board.N * 10; // try 50
    int bestCycleObjective = board.N * board.N; // init to max val
    int lastCycleObjective = board.N * board.N; // init to max val

    int unfixedCount = 0;
    for (auto row : board.fixed) {
        unfixedCount += std::count(row.begin(), row.end(), false);
    }
    int cycleIterations = round(pow(unfixedCount, 2));
    std::cout << "cp will run every " << cycleIterations * 10 << " iterations\n";

    board.fixBlocksWithSingleCellMissing();
    vector<vector<bool>> originalFixedMap(board.fixed);
    cpProcedure(board, resetFactor);
    board.generateSolution();

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
    while (!acceptor->isSolved()) {
//    while(iterations < iterationLimit){
//        if(iterations > iterationLimit)
//            break;
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

        if (iterations % (cycleIterations) == 0) {
            std::cout << "-------------------CP-------------------\n";
            std::cout << "running for: " << cycleIterations * board.n * 10<< " iterations\n";

            lastCycleObjective = acceptor->getObjective();

            if (lastCycleObjective >= bestCycleObjective)
                cpProcedure(board, resetFactor);
            else
                cpProcedure(board, 0);

            bestCycleObjective =
                    lastCycleObjective < bestCycleObjective ? lastCycleObjective : bestCycleObjective;

            unfixedCount = 0;
            for (auto row : board.fixed) {
                unfixedCount += std::count(row.begin(), row.end(), false);
            }
            cycleIterations = round(pow(unfixedCount, 2));

            board.generateSolution();
            acceptor->recalculateObjective(board);

            std::cout << "last: " << lastCycleObjective << " best: " << bestCycleObjective << " worsening cycles " << worseningCycles << " reset_f " << resetFactor << "\n" ;

            if (lastCycleObjective <= bestCycleObjective) {
                bestCycleObjective = lastCycleObjective;
                worseningCycles = 0;

                if (resetFactor > 0.1) {
                    std::cout << "decreasing resetFactor " << resetFactor << "\n";
                    resetFactor *= resetAlpha;
                }
            } else {
                worseningCycles++;

                if (worseningCycles >= worseningCyclesLimit) {
                    board.fixed = originalFixedMap;
                    std::cout << "After cyclic reset in CP\n";
                    board.printBoard();
                    board.randomiseExistingSolution();

                    unfixedCount = 0;
                    for (auto row : board.fixed) {
                        unfixedCount += std::count(row.begin(), row.end(), false);
                    }
                    cycleIterations = round(pow(unfixedCount, 2));

                    resetFactor = 1.0;
                    worseningCycles = 0;
                    bestCycleObjective = acceptor->recalculateObjective(board);
                }
            }

        }
    }

    //print and thoroughly verify final solution
    board.printBoard();
    bool isSolved = board.verifySolved();

    double timeTaken = (double) (clock() - tStart) / CLOCKS_PER_SEC;
    double iterationsPerSecond = iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC);

    printf("\nTotal iterations:     %d\n", selector->getIterations());
    printf("Iterations per second:  %.2f\n", iterations / ((double) (clock() - tStart) / CLOCKS_PER_SEC));
    printf("Time taken:             %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
    printf("Sel and Acc time:       %.2fs\n", (stime + atime) / CLOCKS_PER_SEC);
    printf("Selector time taken:    %.5fs\n", stime / CLOCKS_PER_SEC);
    printf("Acceptor time taken:    %.5fs\n", atime / CLOCKS_PER_SEC);
    selector->printOperatorCounts();

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
