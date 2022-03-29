#include "simulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(boardType &board, Selector &aSelector) : Acceptor(board), selector(aSelector),
                                                                                startingObjective(objective),
                                                                                iterationCount(),
                                                                                worseningIterationCount(),
                                                                                iterationCycle(),
                                                                                restartCount(),
                                                                                isStuck(){

    temperature = calculateTemperature(board) / 100;//calculateTemperature(board, selector)/100;
    initialTemperature = temperature;
    iterationLimit = calculateIterationLimit(board);

    printf("Starting objective:     %d\n", objective);
    printf("Starting temperature:   %f\n", temperature);
}

int SimulatedAnnealing::process(boardType &board) {
    if (iterationCount >= iterationLimit) {
        //std::cout << "UPDATING TEMP " << std::endl;
        //increment or reset bad markov chain count
        worseningIterationCount = objective >= startingObjective ? worseningIterationCount + 1 : 0;

        //determine whether the algorithm is stuck by comparing bad markov chain count to the limit
        isStuck = worseningIterationCount >= WORSENING_ITERATION_LIMIT;

        //update temperature
        temperature *= COOL_RATE;

        //reset markov chain variables
        startingObjective = objective;
        iterationCount = 0;

        iterationCycle++;

        if ((iterationLimit * iterationCycle) % (iterationLimit * 5) == 0) {
            printf("Iteration: %10d\t Objective: %10d\t Temperature: %10f\n",
                   iterationLimit * iterationCycle, objective, temperature);
        }
    }

    if (temperature < TEMPERATURE_THRESHOLD || isStuck) {
        std::cout << "RESETING " << std::endl;
        //shuffle board, flushing move history and recalculating objective
        board.randomiseExistingSolution();
        int objChange = board.calculateObjective() - objective;

        //update local variables
        objective = board.calculateObjective();

        restartCount++;
        temperature = calculateTemperature(board);
        iterationCount = 0;
        isStuck = false;

        return objChange;
    } else {
        //std::cout << "EVALUATING " << std::endl;
        int objChange = board.updateObjective();
        double prob = pow(2.718282, (-objChange / temperature));

        if (objChange <= 0 || prob > (double(fastrand()) / RAND_MAX)) {
            //std::cout << "accepted " << objChange << " with probability " << prob << std::endl;
            objective += objChange;
            board.acceptChange();
        } else {
            //std::cout << "rejected " << objChange << std::endl;
            board.undoChange();
        }

        iterationCount++;
        return objChange;
    }
}

double SimulatedAnnealing::calculateTemperature(boardType &board) {
    if (objective == 0)
        return 0;

    vector<double> samples;
    samples.push_back(objective);

    //collect samples by observing the objective after simulating a single move
    for (int i = 1; i <= SAMPLE_SIZE; i++) {
        //make a single change to the board
        selector.select(board);
        //observe the change in objective
        int change = board.updateObjective();
        //sample the total objective after change
        samples.push_back(objective + change);
        //return to starting state
        board.undoChange();
    }

    double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
    double mean = sum / samples.size();

    std::vector<double> diff(samples.size());
    std::transform(samples.begin(), samples.end(), diff.begin(), [mean](double x) { return x - mean; });

    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / samples.size());
}

int SimulatedAnnealing::calculateIterationLimit(boardType &board) {
    //iterationCount limit will be unfixed_count ^ 2
    int unfixedCount = 0;
    for (auto row : board.fixed) {
        unfixedCount += std::count(row.begin(), row.end(), false);
    }

    return round(pow(unfixedCount, 2));
}

int SimulatedAnnealing::recalculateObjective(boardType &board) {
    temperature = calculateTemperature(board)/100;
    iterationCount = 0;
    isStuck = false;
    iterationLimit = calculateIterationLimit(board);

    startingObjective = Acceptor::recalculateObjective(board);
    return objective;
}

