#include "greedy.h"

Greedy::Greedy() {
    bestOperator = 0;
    //init score to lowest value
    bestScore = 0xFF;
}

void Greedy::select(boardType &board) {
    bestOperator = 0;
    //init score to lowest value
    bestScore = 0xFF;

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
    useCounts[bestOperator]++;
    iterations++;
}

void Greedy::updateState(int change) {

}