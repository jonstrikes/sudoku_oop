#include "onlyImprove.h"

OnlyImprove::OnlyImprove(boardType &board) : Acceptor(board){

}

int OnlyImprove::process(boardType &board) {
    //reevaluate board
    int objChange = board.updateObjective();

    if(objChange < 0) {
        objective += objChange;
        board.acceptChange();
    } else {
        board.undoChange();
    }

    return objChange;
}