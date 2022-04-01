#include "improveOrEqual.h"

ImproveOrEqual::ImproveOrEqual(boardType &board) : Acceptor(board){

}

int ImproveOrEqual::process(boardType &board) {
    iteration++;
    int objChange = recalculate(board);

    if(objChange > 0) {
        reject(board);
    } else {
        accept(board, objChange);
    }

    return objChange;
}

void ImproveOrEqual::printShortLog() {
    printf("\nIOE acceptance strategy, iteration: %d\n", iteration);
    printf("Current Objective: %d\n", objective);
}
