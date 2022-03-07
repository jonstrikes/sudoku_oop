#include "acceptor.h"

Acceptor::Acceptor(boardType &board) {
    objective = calcObj(&board);
}

bool Acceptor::isSolved() const {
    return objective == 0;
}
