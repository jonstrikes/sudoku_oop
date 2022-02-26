#ifndef SUDOKU_OOP_ACCEPTOR_H
#define SUDOKU_OOP_ACCEPTOR_H

#include "../board.h"
#include "../Solver.h"

class Acceptor
{
protected:
    int objective;
public:
    Acceptor(boardType board);
    virtual ~Acceptor() {}
    virtual void process(boardType &board) = 0;
    bool isSolved();
};

#endif //SUDOKU_OOP_ACCEPTOR_H
