#include "acceptor.h"

Acceptor::Acceptor(boardType &board) : timeSpentRecalculating(), timeSpentAccepting(), timeSpentRejecting(),
                                       recalcCallCount(), acceptCallCount(), rejectCallCount(), iteration() {
    objective = board.calculateObjective();
}

void Acceptor::accept(boardType &board, int objChange) {
    acceptCallCount++;
    clock_t t = clock();
    objective += objChange;
    board.acceptChange();
    timeSpentAccepting += (double)(clock() - t);
}

void Acceptor::reject(boardType &board) {
    rejectCallCount++;
    clock_t t = clock();
    board.undoChange();
    timeSpentRejecting += (double)(clock() - t);
}

int Acceptor::recalculate(boardType &board) {
    recalcCallCount++;
    clock_t t = clock();
    int objChange = board.updateObjective();
    timeSpentRecalculating += (double)(clock() - t);
    return objChange;
}

bool Acceptor::isSolved() const {
    return objective == 0;
}

int Acceptor::resetState(boardType &board) {
    objective = board.calculateObjective();
    return objective;
}

int Acceptor::getObjective() {
    return objective;
}

void Acceptor::printLog() {
    printf("=========== Acceptor ===========\n");
    printf("Objective score: %d\n\n", objective);

    printf("Total calls to operations:\n");
    printf("%-20.15s %d\n", "Accept:", acceptCallCount);
    printf("%-20.15s %d\n", "Reject:", rejectCallCount);
    printf("%-20.15s %d\n", "Recalculate:", recalcCallCount);

    printf("\nTotal time spent on acceptance procedures:\n");
    printf("%-20.15s %.5es\n", "Accept:", timeSpentAccepting / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "Reject:", timeSpentRejecting / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "Recalculate:", timeSpentRecalculating / CLOCKS_PER_SEC);

    printf("\nAverage time spent on acceptance procedures:\n");
    printf("%-20.15s %.5es\n", "Accept:", timeSpentAccepting / (acceptCallCount * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "Reject:", timeSpentRejecting / (rejectCallCount * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "Recalculate:", timeSpentRecalculating / (recalcCallCount * CLOCKS_PER_SEC));

}

std::string Acceptor::getLog() {
    //join current iteration count and objective
    std::string res = std::to_string(iteration) + " " + std::to_string(objective) + " ";

    //join call counts
    res += std::to_string(acceptCallCount) + " " + std::to_string(rejectCallCount) + " " +
           std::to_string(recalcCallCount) + " ";

    //join total time spent in seconds
    res += std::to_string(timeSpentAccepting / CLOCKS_PER_SEC) + " " +
           std::to_string(timeSpentRejecting / CLOCKS_PER_SEC) + " " +
           std::to_string(timeSpentRecalculating / CLOCKS_PER_SEC) + " ";

    res += std::to_string(timeSpentAccepting / (acceptCallCount * CLOCKS_PER_SEC)) + " " +
           std::to_string(timeSpentRejecting / (rejectCallCount * CLOCKS_PER_SEC)) + " " +
           std::to_string(timeSpentRecalculating / (recalcCallCount * CLOCKS_PER_SEC)) + " ";

    return res + "\n";
}
