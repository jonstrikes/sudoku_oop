#include "adaptiveIterationLimitedThresholdAccepting.h"

AdaptiveIterationLimitedThresholdAccepting::AdaptiveIterationLimitedThresholdAccepting(
        boardType &board, int W_ITERATION_THRESHOLD,double K_FACTOR, double E_INITIAL, double E_FACTOR)
        :
        Acceptor(board), w_iterations(), iteration(), e(E_INITIAL),
        W_ITERATION_THRESHOLD(W_ITERATION_THRESHOLD), K_FACTOR(K_FACTOR),
        E_INITIAL(E_INITIAL), E_FACTOR(E_FACTOR)
{

    int unfixedCount = board.countUnfixedCells();
    k = pow(unfixedCount, 2) * K_FACTOR;
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
        if(w_iterations != 0 && w_iterations > W_ITERATION_THRESHOLD){
            e *= E_FACTOR;
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
    e = E_INITIAL;
}




