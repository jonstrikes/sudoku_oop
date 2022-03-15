#include "simulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(boardType &board, Selector &selector) : Acceptor(board) {
    startingObjective = objective;
    iterationCount = 0;
    worseningIterationCount = 0;
    improvingIterationCount = 0;
    restartCount = 0;
    isStuck = false;

    temperature = calculateInitialTemperature(board, selector);
    initialTemperature = temperature;
    iterationLimit = calculateIterationLimit(board);

    printf("Starting objective:     %d\n", objective);
    printf("Starting temperature:   %f\n", temperature);
}

int SimulatedAnnealing::process(boardType &board) {
    if(iterationCount < iterationLimit) {
        //std::cout << "EVALUATING " << std::endl;
        int objChange = board.updateObjective();
        double prob = pow(2.718282, (-objChange / temperature));

        if (objChange <= 0 || prob > (fastrand() / double(RAND_MAX))) {
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
    else if(temperature < TEMPERATURE_THRESHOLD || isStuck){
        std::cout << "RESETING " << std::endl;
        //shuffle board, flushing move history and recalculating objective
        board.randomiseExistingSolution();
        board.acceptChange();
        int objChange = board.calculateObjective() - objective;

        //update local variables
        objective = board.calculateObjective();

        restartCount++;
        temperature = initialTemperature;
        iterationCount = 0;
        isStuck = false;

        return objChange;
    }
    else{
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

        improvingIterationCount++;

        if((iterationLimit * improvingIterationCount) % (iterationLimit * 5) == 0){
            printf("Iteration: %10d\t Objective: %10d\t Temperature: %10f\n",
                   iterationLimit * improvingIterationCount, objective, temperature);
        }

        return 0;
    }
}

double SimulatedAnnealing::calculateInitialTemperature(boardType &board, Selector &selector) {
    vector<double> samples;
    samples.push_back(objective);

    //collect samples by observing the objective after simulating a single move
    for(int i=1; i<=SAMPLE_SIZE; i++){
        //make a single change to the board
        selector.select(board);
        //observe the change in objective
        int change = board.updateObjective();
        //add that change to original objective
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
    for(auto row : board.fixed){
        unfixedCount += std::count(row.begin(), row.end(), false);
    }

    return round(pow(unfixedCount, 2));
}
