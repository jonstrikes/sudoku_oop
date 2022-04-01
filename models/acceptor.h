#ifndef SUDOKU_OOP_ACCEPTOR_H
#define SUDOKU_OOP_ACCEPTOR_H

#include "../board.h"
/**
 * The acceptor class serves as the base class for all implementations. It is used as an interface to implement
 * acceptance strategies that can combine fluently with selection methods.
 *
 * The structure is intended to centralise acceptor behaviour while also allowing support
 * for more advanced techniques like SA or AILTA that require more complex logic.
 *
 * An implemented class is expected to override process, resetState and printShortLog methods.
 **/
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

public:
    explicit Acceptor(boardType &board);
    virtual ~Acceptor() {};

    //decide whether to accept or reject change based on some strategy
    virtual int process(boardType &board) = 0;
    [[nodiscard]] bool isSolved() const;

    //revert to some "safe" state
    virtual int resetState(boardType &board);
    int getObjective();

    //is intended to provide acceptor-specific output for the user
    virtual void printShortLog() = 0;

    //general data collected for all implementing acceptors
    void printLog();
    std::string getLog();
};

#endif //SUDOKU_OOP_ACCEPTOR_H
