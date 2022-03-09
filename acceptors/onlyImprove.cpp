#include "onlyImprove.h"

OnlyImprove::OnlyImprove(boardType &board) : Acceptor(board){

}

int OnlyImprove::process(boardType &board) {
    int objChange = board.updateObjective();

    if(objChange < 0) {
        std::cout << "accepted " << objChange;
        objective += objChange;
        board.acceptChange();
    } else {
        std::cout << "rejected " << objChange;
        board.undoChange();
    }

    std::cout << "\nAfter obj change of: " << objChange << std::endl;
    std::cout << "Total objective: " << objective << std::endl << std::endl;
    //std::cout << "Actual objective " << board.calculateObjective() << std::endl << std::endl;

    return objChange;
}