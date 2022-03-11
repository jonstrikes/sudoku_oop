#include "improveOrEqual.h"

ImproveOrEqual::ImproveOrEqual(boardType &board) : Acceptor(board){
    timeSpentRecalculating = 0;
    timeSpentUndoing = 0;
    timeSpentAccepting = 0;
    iterations = 0;

    acceptCalled = 0, recalcCalled = 0, undoCalled = 0;
}

int ImproveOrEqual::process(boardType &board) {
    iterations++;

        recalcCalled++;
        clock_t s1 = clock();
    int objChange = board.updateObjective();
        timeSpentRecalculating += (double)(clock() - s1);

    if(objChange > 0) {
        //std::cout << "rejected " << objChange;
            undoCalled++;
            clock_t s2 = clock();
        board.undoChange();
            timeSpentUndoing += (double)(clock() - s2);
    } else {
        //std::cout << "accepted " << objChange;
        objective += objChange;
            acceptCalled++;
            clock_t s3 = clock();
        board.acceptChange();
            timeSpentAccepting += (double)(clock() - s3);
    }

    if(iterations % 100000 == 0){
        //print acceptor times
        printf("Accepting time spent: %.5fs\n", timeSpentAccepting/CLOCKS_PER_SEC);
        printf("Undoing time spent: %.5fs\n", timeSpentUndoing/CLOCKS_PER_SEC);
        printf("Recalculating time spent: %.5fs\n", timeSpentRecalculating/CLOCKS_PER_SEC);

        printf("Average accept time: %.5f\n", timeSpentAccepting/acceptCalled);
        printf("Average undo time: %.5f\n", timeSpentUndoing/undoCalled);
        printf("Average recalc time: %.5f\n", timeSpentRecalculating/recalcCalled);

        printf("Call counts:\nAccept: %d\t Undo: %d\t Recalc: %d\t \n", acceptCalled, undoCalled, recalcCalled);
        printf("Total time:\nAccept: %f\t Undo: %f\t Recalc: %f\t \n", timeSpentAccepting, timeSpentUndoing, timeSpentRecalculating);

        std::cout << "Total objective: " << objective << std::endl;
        std::cout << "Actual objective " << board.calculateObjective() << std::endl << std::endl;
    }

//    std::cout << "\nAfter obj change of: " << objChange << std::endl;
//    std::cout << "Total objective: " << objective << std::endl << std::endl;
//    std::cout << "Actual objective " << board.calculateObjective() << std::endl << std::endl;

    return objChange;
}