#include "simulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(boardType &board, Selector &selector,
                                       const double COOL_RATE, const double TEMPERATURE_THRESHOLD,
                                       const double TEMPERATURE_FACTOR,
                                       const int SAMPLE_SIZE, const int NON_IMPROVING_CYCLE_LIMIT,
                                       const double CYCLE_LENGTH_FACTOR)
        : Acceptor(board), selector(selector),
          COOL_RATE(COOL_RATE), TEMPERATURE_THRESHOLD(TEMPERATURE_THRESHOLD), TEMPERATURE_FACTOR(TEMPERATURE_FACTOR),
          SAMPLE_SIZE(SAMPLE_SIZE), NON_IMPROVING_CYCLE_LIMIT(NON_IMPROVING_CYCLE_LIMIT), CYCLE_LENGTH_FACTOR(CYCLE_LENGTH_FACTOR),
          startingObjective(objective),
          iterationCount(),
          iterationCycle(),
          cyclesWithoutImprovement(),
          restartCount(),
          isStuck() {

    temperature = calculateTemperature(board);
    iterationLimit = calculateIterationLimit(board);

    printf("Starting objective:     %d\n", objective);
    printf("Starting temperature:   %f\n", temperature);
}

int SimulatedAnnealing::process(boardType &board) {
    if (iterationCount >= iterationLimit) {
        std::cout << "UPDATING TEMP " << std::endl;
        //increment or reset bad markov chain count
        cyclesWithoutImprovement = objective >= startingObjective ? cyclesWithoutImprovement + 1 : 0;

        //determine whether the algorithm is stuck by comparing bad markov chain count to the limit
        isStuck = cyclesWithoutImprovement >= NON_IMPROVING_CYCLE_LIMIT;

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

        temperature = calculateTemperature(board);
        restartCount++;
        iterationCount = 0;
        isStuck = false;

        return objChange;
    } else {
        //std::cout << "EVALUATING " << std::endl;
        int objChange = board.updateObjective();
        double prob = pow(2.718282, (-objChange / temperature));

        //always accept improving and probabilistically worsening move
        if (objChange <= 0 || prob > (double(fastrand()) / RAND_MAX)) {
            objective += objChange;
            board.acceptChange();
        } else {
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
    return std::sqrt(sq_sum / samples.size()) * TEMPERATURE_FACTOR;
}

int SimulatedAnnealing::calculateIterationLimit(boardType &board) {
    int unfixedCount = board.countUnfixedCells();

    return round(pow(unfixedCount, 2)) * CYCLE_LENGTH_FACTOR;
}

int SimulatedAnnealing::recalculateObjective(boardType &board) {
    iterationCount = 0;
    isStuck = false;
    temperature = calculateTemperature(board);
    iterationLimit = calculateIterationLimit(board);

    startingObjective = Acceptor::recalculateObjective(board);
    return objective;
}

