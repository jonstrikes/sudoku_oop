#ifndef SUDOKU_OOP_ACCEPTOR_H
#define SUDOKU_OOP_ACCEPTOR_H

#include "../board.h"
#include "../Solver.h"

class Acceptor
{
protected:
    int objective;
public:
    explicit Acceptor(boardType &board);
    virtual ~Acceptor() {}
    virtual int process(boardType &board) = 0;
    [[nodiscard]] bool isSolved() const;
};

#endif //SUDOKU_OOP_ACCEPTOR_H
