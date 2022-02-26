#include "acceptor.h"

Acceptor::Acceptor(boardType board) {
    objective = calcObj(&board);
}

bool Acceptor::isSolved() {
    return objective == 0;
}
