#include "acceptor.h"

Acceptor::Acceptor(boardType &board) {
    objective = board.calculateObjective();
}

bool Acceptor::isSolved() const {
    return objective == 0;
}
