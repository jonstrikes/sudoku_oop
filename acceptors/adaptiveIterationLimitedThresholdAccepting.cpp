#include "adaptiveIterationLimitedThresholdAccepting.h"

/**
 * This selection method was implemented after reading
 *  "Hyper-heuristics with a dynamic heuristic set for the home care scheduling problem"
 *  (Misir, Mustafa)
 **/
AdaptiveIterationLimitedThresholdAccepting::AdaptiveIterationLimitedThresholdAccepting(
        boardType &board, int W_ITERATION_THRESHOLD, double K_FACTOR, double E_INITIAL, double E_FACTOR)
        :
        Acceptor(board), w_iterations(), iteration(), e(E_INITIAL),
        W_ITERATION_THRESHOLD(W_ITERATION_THRESHOLD), K_FACTOR(K_FACTOR),
        E_INITIAL(E_INITIAL), E_FACTOR(E_FACTOR) {

    int unfixedCount = board.countUnfixedCells();
    k = pow(unfixedCount, 2) * K_FACTOR;
}

int AdaptiveIterationLimitedThresholdAccepting::process(boardType &board) {
    iteration++;
    //reevaluate board
    int objChange = recalculate(board);

    if (objChange <= 0) {
        if (objChange < 0) {
            resetParameters();
        }
        accept(board, objChange);
    } else {
        if (w_iterations != 0 && w_iterations > W_ITERATION_THRESHOLD) {
            e *= E_FACTOR;
        }
        if (w_iterations >= k && (objChange < objective * e)) {
            accept(board, objChange);

            resetParameters();
        } else {
            w_iterations++;
            reject(board);
        }
    }

    return objChange;
}

void AdaptiveIterationLimitedThresholdAccepting::resetParameters() {
    w_iterations = 0;
    e = E_INITIAL;
}

void AdaptiveIterationLimitedThresholdAccepting::printShortLog() {
    printf("\nAILTA acceptance strategy, iteration: %d\n", iteration);
    printf("k: %d\te: %f\tworsening iterations: %d\n", k, e, w_iterations);
    printf("objective: %d\tCurrent threshold: %f.2\n", objective, objective * e);
}




