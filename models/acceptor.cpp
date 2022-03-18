#include "acceptor.h"

Acceptor::Acceptor(boardType &board) {
    objective = board.calculateObjective();
}

bool Acceptor::isSolved() const {
    return objective == 0;
}

std::string Acceptor::getLog() {
    std::string res = std::to_string(objective) + "\n";

    return res;
}
