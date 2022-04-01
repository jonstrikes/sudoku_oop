Hyperheuristics to solve Sudoku by Jonas Streckis

Code was compiled and ran on an Intel I5-7300HQ 2.50GHz Windows 10 machine running MinGW 5.4 GCC 9.2.0
The project was compiled and ran using the CMakeLists file provided in Release mode with -o3 optimisation turned on.

=======================================================================================================================

The program works by taking a single puzzle as input and produces output to four files that log different parts of the algorithm.

It is important that relative paths are used when providing input, output file paths and that the specs.json file
is included in the compiled directory for the program to work correctly.

Required input arguments:
    puzzleFile - path to input puzzle
    outputSolutionFile - path to solution output file
    outputGeneralLogFile - path to general log output file
    outputAcceptorLogFile - path to acceptor log output file
    outputSelectorLogFile - path to selector log output file

A Hyper-heuristic can be chosen by supplying two optional parameters:
    -acceptor_method    -   specifies the move acceptance strategy. The acceptor methods available are:
        Only Improve (OI):            --only-improve, -oi
        Improve or Equal (IOE):       --improve-or-equal, -ioe, -ie
        Simulated Annealing (SA):     --simulated-annealing, -sa
        Adaptive Iteration Limited Threshold Accepting (AILTA):
                                      --adaptive-iteration-limited-threshold-accepting, --ailta

    -selection_method   -   specifies the move selection strategy. The list of selection methods are:
         Simple Random (SR):          --simple-random, -sr
         Random Descent (RD):         --random-descent, -rd
         Random Permutation (RP):     --random-permutation, -rp
         Random Permutation Descent (RPD):
                                      --random-permutation-descent, -rpd
         Reinforcement Learning(RL):  --reinforcement-learning, -rl

A single example run of the program:
./cmake-build-release sudoku_oop.exe data/puzzle1.txt ./res/puzzle1_sol.txt ./res/gen_log.txt ./res/acc_log.txt ./res/sel_log.txt -sa, -sr

=======================================================================================================================
Configuration:

To easier parse parameters, an external library https://nlohmann.github.io/json/ was used to parse JSON.
No other external libraries were used for this implementation.

=======================================================================================================================
Parameters:

specs.json contains many of the adjustable parameters that define essential parts of execution. The exact Hyper-heuristic
is built at run time. Any parameters that can be changed without modifying code can be found
in specs.json. Some were not used and will not be included in the report.

General parameters:
    SEED - the value that is used to seed fastRand()
    LOG_ITERATIONS_FACTOR - relative factor controlling how often logging messages are produced to standard output
    Time_limits - set the maximum time in seconds for a given run based on the input problem size.

Simulated Annealing:
    SAMPLE_SIZE - how many operations are sampled when calculating temperature
    NON_IMPROVING_CYCLE_LIMIT - controls how many cycles can pass without improvement to objective score before resetting solution
    COOL_RATE - controls the rate of change in temperature
    TEMPERATURE_THRESHOLD - sets the minimum temperature threshold before resetting solution
    TEMPERATURE_FACTOR - is used to scale the calculated temperature value
    CYCLE_LENGTH_FACTOR - is used to scale the length of a cycle relative to problem size

Reinforcement_Learning":
    UTILITY_UPPER_BOUND_FACTOR" - scales the utility upper bound value
    UTILITY_INITIAL_FACTOR": - initial utility value of all operators relative to upper bound
    PENALTY_METHOD" - the three penalty methods implemented are subtractive (P1), divisional (P2), root (P3)
    REWARD_METHOD" - the three reward methods implemented are additive (R1), multiplicative (R2), power (R3)

    The six negative/positive reinforcement strategies implemented as described in
    "CHOOSING SEARCH HEURISTICS BY NONSTATIONARY REINFORCEMENT LEARNING" (Nareyek, Alexander)

Adaptive_Iteration_Limited_Threshold_Accepting":
    W_ITERATION_THRESHOLD - controls how greedy the algorithm is
    K_FACTOR - can be used to scale k value
    E_INITIAL - initial setting of e value
    E_FACTOR - can be used to scale e value


In addition to local search, a constraint programming procedure clears part of the board and attempts to fill empty cells implicitly.
This procedure is run periodically and is used to help guide local search methods
as well as escape from local optima solutions.
    CYCLE_ITERATIONS_FACTOR - controls how many cycles pass before the procedure is run
    LOG_CYCLE_LIMIT - controls how often short logging messages are displayed to std_out
    WORSENING_CYCLES_LIMIT - controls how many worsening cycles are allowed before a solution is randomised
    WORSENING_CYCLES_FACTOR - scales the worsening cycle limit based on problem size
    RESET_INITIAL - initial value of resetFactor
    RESET_MIN - the minimum value resetFactor can have
    RESET_MAX - the maximum value resetFactor can have
    RESET_ALPHA - controls resetFactor


=======================================================================================================================
Running experiments:

The data set for experiments is one used in
"A Hybrid Approach for the Sudoku problem: Using Constraint Programming in Iterated Local Search" (Musliu, Winter)

Experiments can be run with the help ./run_experiments.sh bash script provided.
Note, that this script runs the program 756000 times covering the entire dataset with every possible hyper-heuristic
combination and produces many output files.

Experiments can be run via:
bash ./run_experiments

