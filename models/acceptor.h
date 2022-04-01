#ifndef SUDOKU_OOP_ACCEPTOR_H
#define SUDOKU_OOP_ACCEPTOR_H

#include "../board.h"

class Acceptor
{
protected:
    int iteration;
    int objective;

    //centralised methods for sub-classes that take care of logging variables
    int recalculate(boardType &board);
    void accept(boardType &board, int objChange);
    void reject(boardType &board);

    //logging variables
    double timeSpentRecalculating;
    double timeSpentRejecting;
    double timeSpentAccepting;
    int recalcCallCount;
    int rejectCallCount;
    int acceptCallCount;

    const int LOG_ITERATIONS_LIMIT = 10000;

public:
    explicit Acceptor(boardType &board);
    virtual ~Acceptor() {};

    virtual int process(boardType &board) = 0;
    [[nodiscard]] bool isSolved() const;
    virtual int resetState(boardType &board);
    int getObjective();

    virtual void printShortLog() = 0;

    void printLog();
    std::string getLog();
};

#endif //SUDOKU_OOP_ACCEPTOR_H
