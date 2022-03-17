#include "adaptiveIterationLimitedThresholdAccepting.h"

AdaptiveIterationLimitedThresholdAccepting::AdaptiveIterationLimitedThresholdAccepting(boardType &board) : Acceptor(board){
    wIterationThreshold = 100000;
    unfixedCount = 0;
    for(auto row : board.fixed){
        unfixedCount += std::count(row.begin(), row.end(), false);
    }

    k = pow(unfixedCount, 2);
    iteration = 0;
    w_iterations = 0;
    e = 0.3;

}

int AdaptiveIterationLimitedThresholdAccepting::process(boardType &board) {
    //reevaluate board
    int objChange = board.updateObjective();

    if(iteration % 1000000 == 0) {
        std::cout << "k: " << k <<" w_iterations: " << w_iterations << std::endl;
        std::cout << "ObjChange: " << objChange << " Threshold: " << objective * e << std::endl;
        std::cout << "Objective: " << objective << " Threshold: " << objective + objective * e << std::endl << std::endl;
    }

    if(objChange <= 0) {
        if(objChange < 0) {
            resetParameters();
        }

        objective += objChange;
        board.acceptChange();
    } else {
        if(w_iterations != 0 && w_iterations % wIterationThreshold == 0){
            e *= 20;
            std::cout << "resst called "<< e << " threshold: "<< objective * e << std::endl;
        }
        if(w_iterations >= k && (objChange < objective * e)){
            objective += objChange;
            board.acceptChange();

            resetParameters();
        } else{
            w_iterations++;
            board.undoChange();
        }
    }

    iteration++;
    return objChange;
}

void AdaptiveIterationLimitedThresholdAccepting::resetParameters() {
    w_iterations = 0;
    e = 0.3;
}




