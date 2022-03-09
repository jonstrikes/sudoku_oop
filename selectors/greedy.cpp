#include "greedy.h"

Greedy::Greedy() {
    bestOperator = 0;
    bestScore = std::numeric_limits<int>::max();
}

void Greedy::select(boardType &board) {
    bestOperator = 0;
    bestScore = std::numeric_limits<int>::max();

    for(int i=0; i<operators.size(); i++){
        //apply all operators
        (*operators[i])(board);

        //find best value
        int currentScore = board.updateObjective();
        if(currentScore < bestScore){
            bestOperator = i;
            bestScore = currentScore;
        }

        //reset board for next iterationCount
        board.undoChange();
    }

    //apply the best
    (*operators[bestOperator])(board);
}

void Greedy::updateState(int change) {

}