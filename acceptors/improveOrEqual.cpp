#include "improveOrEqual.h"

ImproveOrEqual::ImproveOrEqual(boardType &board) : Acceptor(board){
}

int ImproveOrEqual::process(boardType &board) {
    int objChange = board.updateObjective();

    if(objChange > 0) {
        std::cout << "rejected " << objChange;
        board.undoChange();
    } else {
        std::cout << "accepted " << objChange << std::endl;
        objective += objChange;
        board.acceptChange();
    }

    std::cout << "\nAfter obj change of: " << objChange << std::endl;
    std::cout << "Total objective: " << objective << std::endl;
    //std::cout << "Actual objective " << board.calculateObjective() << std::endl << std::endl;

    return objChange;
}