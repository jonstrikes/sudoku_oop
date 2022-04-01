#include "onlyImprove.h"

OnlyImprove::OnlyImprove(boardType &board) : Acceptor(board){

}

int OnlyImprove::process(boardType &board) {
    iteration++;
    int objChange = recalculate(board);

    if(objChange < 0) {
        accept(board, objChange);
    } else {
        reject(board);
    }

    return objChange;
}

void OnlyImprove::printShortLog() {
    printf("\nOI acceptance strategy, iteration: %d\n", iteration);
    printf("Current Objective: %d\n", objective);
}
