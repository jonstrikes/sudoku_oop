#include "SudokuIO.h"

using std::string;
using std::cout;

boardType readFile(const string &fileDir) {
    std::ifstream inFile;
    inFile.open(fileDir);

    if (inFile.fail()) {
        cout << "Error: " << strerror(errno) << std::endl;
        cout << "Could not find input problem file" << std::endl;
    }

    int n, N, minCellValue, maxCellValue;
    inFile >> n >> minCellValue;

    N = n * n;
    maxCellValue = minCellValue + N - 1;

    //initialise grid and fixed
    vector<vector<int>> grid(N, vector<int>(N));
    vector<vector<bool>> fixed(N, vector<bool>(N));

    //all benchmark files contain one puzzle
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int current_cell;
            inFile >> current_cell;
            grid[row][col] = current_cell;
            fixed[row][col] = current_cell != -1;
        }
    }

    return boardType(n, N, minCellValue, maxCellValue, grid, fixed);
}


nlohmann::json readSpecification() {
    std::ifstream specsFile("specs.json", std::ifstream::binary);

    if (specsFile.fail()) {
        cout << "Error: " << strerror(errno) << std::endl;
        cout << "Could not find specs.json file" << std::endl;
    }

    nlohmann::json specs;
    specsFile >> specs;

    specsFile.close();
    return specs;
}

bool readCMDParams(char **input, int size, std::string &puzzleDir) {
    if (size != 2 && size != 4) {
        printf("Sudoku Solver\n");
        printf("Usage: puzzlePath -acceptor -selector\n");
        printf("If either the acceptor or selector is not specified, the program defaults to Simple Random and Improve or Equal\n");
        printf("Acceptors: -oi, -ie, -sa, -ailta\n");
        printf("Selectors: -sr, -rd, -rp, -rpd\n");
        return false;
    }

    puzzleDir = input[1];

    return true;
}

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType) {
    if (size != 4) {
        return false;
    }
    acceptorType = input[2];
    selectorType = input[3];
    return true;
}

void readAcceptorMethod(const std::string &acceptorMethod, nlohmann::json &specs,
                        Acceptor *&acceptor, Selector *&selector, boardType &board)
{
    if (acceptorMethod == "--only-improve" || acceptorMethod == "-oi") {
        acceptor = new OnlyImprove(board);
    } else if (acceptorMethod == "--improve-or-equal" || acceptorMethod == "--improve-equal" ||
               acceptorMethod == "-ie" || acceptorMethod == "-ioe") {
        acceptor = new ImproveOrEqual(board);
    } else if (acceptorMethod == "--simulated-annealing" || acceptorMethod == "-sa") {
        int SAMPLE_SIZE, NON_IMPROVING_CYCLE_LIMIT;
        double COOL_RATE, TEMPERATURE_THRESHOLD, TEMPERATURE_FACTOR, CYCLE_LENGTH_FACTOR;

        try {
            SAMPLE_SIZE = specs["Simulated_Annealing"]["SAMPLE_SIZE"];
            NON_IMPROVING_CYCLE_LIMIT = specs["Simulated_Annealing"]["NON_IMPROVING_CYCLE_LIMIT"];
            COOL_RATE = specs["Simulated_Annealing"]["COOL_RATE"];
            TEMPERATURE_THRESHOLD = specs["Simulated_Annealing"]["TEMPERATURE_THRESHOLD"];
            TEMPERATURE_FACTOR = specs["Simulated_Annealing"]["TEMPERATURE_FACTOR"];
            CYCLE_LENGTH_FACTOR = specs["Simulated_Annealing"]["CYCLE_LENGTH_FACTOR"];

            acceptor = new SimulatedAnnealing(board, *selector,
                                              COOL_RATE, TEMPERATURE_THRESHOLD, TEMPERATURE_FACTOR,
                                              SAMPLE_SIZE, NON_IMPROVING_CYCLE_LIMIT, CYCLE_LENGTH_FACTOR);

        } catch (std::exception const &ex) {
            throw std::invalid_argument("Could not parse Simulated Annealing parameters from specs.json\n");
        }
    } else if (acceptorMethod == "--adaptive-iteration-limited-threshold-accepting" || acceptorMethod == "-ailta") {
        int W_ITERATION_THRESHOLD;
        double K_FACTOR, E_INITIAL, E_FACTOR;

        try {
            W_ITERATION_THRESHOLD = specs["Adaptive_Iteration_Limited_Threshold_Accepting"]["W_ITERATION_THRESHOLD"];
            K_FACTOR = specs["Adaptive_Iteration_Limited_Threshold_Accepting"]["K_FACTOR"];
            E_INITIAL = specs["Adaptive_Iteration_Limited_Threshold_Accepting"]["E_INITIAL"];
            E_FACTOR = specs["Adaptive_Iteration_Limited_Threshold_Accepting"]["E_FACTOR"];

            acceptor = new AdaptiveIterationLimitedThresholdAccepting(board, W_ITERATION_THRESHOLD, K_FACTOR,
                                                                      E_INITIAL, E_FACTOR);
        } catch (std::exception const &ex) {
            throw std::invalid_argument("Could not parse AILTA parameters from specs.json\n");
        }
    } else {
        acceptor = new ImproveOrEqual(board);
        printf("Acceptor %s not found, using default: Improve or Equal\n", acceptorMethod.c_str());
    }
}

void readSelectorMethod(const std::string &selectorMethod, nlohmann::json &specs, Selector *&selector) {
    if (selectorMethod == "--simple-random" || selectorMethod == "-sr") {
        selector = new SimpleRandom();
    } else if (selectorMethod == "--random-descent" || selectorMethod == "-rd") {
        selector = new RandomDescent();
    } else if (selectorMethod == "--random-permutation" || selectorMethod == "-rp") {
        selector = new RandomPermutation();
    } else if (selectorMethod == "--random-permutation-descent" || selectorMethod == "-rpd") {
        selector = new RandomPermutationDescent();
    } else if (selectorMethod == "--reinforcement-learning" || selectorMethod == "-rl") {
        int UTILITY_UPPER_BOUND_FACTOR;
        double UTILITY_INITIAL_FACTOR;
        std::string PENALTY_METHOD, REWARD_METHOD;

        try {
            UTILITY_UPPER_BOUND_FACTOR = specs["Reinforcement_Learning"]["UTILITY_UPPER_BOUND_FACTOR"];
            UTILITY_INITIAL_FACTOR = specs["Reinforcement_Learning"]["UTILITY_INITIAL_FACTOR"];
            PENALTY_METHOD = specs["Reinforcement_Learning"]["PENALTY_METHOD"];
            REWARD_METHOD = specs["Reinforcement_Learning"]["REWARD_METHOD"];

            selector = new ReinforcementLearning(UTILITY_UPPER_BOUND_FACTOR, UTILITY_INITIAL_FACTOR,
                                                 PENALTY_METHOD, REWARD_METHOD);
        } catch (std::exception const &ex) {
            throw std::invalid_argument("Could not parse Reinforcement Learning parameters from specs.json\n");
        }
    } else {
        selector = new SimpleRandom();
        printf("Selector %s not found, using default: Simple Random\n", selectorMethod.c_str());
    }
}

void prepareOutput(const string &inputPath, string &outputPath, string &fileName, string &runId,
                   const string selectorMethod, const string acceptorMethod) {
    //remove extension from input file path
    string path = inputPath.substr(0, inputPath.find_last_of('/'));

    //extract file name and extension from input path
    string name = inputPath.substr(inputPath.find_last_of('/'), inputPath.find_last_of('.'));
    string extension = name.substr(name.find_last_of('.') + 1, name.size());
    fileName = name.substr(1, name.find_last_of('.') - 1);

    //mirror output path to match input and append hyper-heuristic name used and experiment id
    outputPath = "./program-output/" + path + "/" + acceptorMethod + "_" + selectorMethod + "/" + fileName;

    //generate unique run id to avoid overwriting existing experiment data
    int uniqueId = 0;
    while (std::filesystem::exists(outputPath + "/" + std::to_string(uniqueId)) ||
           std::filesystem::is_directory(outputPath + "/" + std::to_string(uniqueId))) {
        uniqueId++;
    }
    runId = std::to_string(uniqueId);
}

void writeSolution(boardType &board, const string &outputPath, const string &fileName, const string &runId) {
    try {
        //mirror path for output
        if (!std::filesystem::is_directory(outputPath + "/" + runId) ||
            !std::filesystem::exists(outputPath + "/" + runId)) {
            std::filesystem::create_directories(outputPath + "/" + runId);
        }

        //create/overwrite and open
        std::ofstream solution;
        solution.open(outputPath + "/" + runId + "/" + fileName + "_sol.txt", std::ofstream::trunc);

        //write order and min value as standard format
        solution << board.n << "\n";
        solution << board.minCellValue << "\n";

        //write out board;
        for (int i = 0; i < board.N; i++) {
            for (int j = 0; j < board.N; j++) {
                solution << board.board[i][j] << " ";
            }
            solution << "\n";
        }

        solution.close();

    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

void writeSelectorLog(boardType &board, Selector *&selector, const std::string &outputPath, const std::string &fileName,
                      const std::string &runId) {
    //maybe make all experiments write general data into a single file
    try {
        //mirror path for output
        if (!std::filesystem::is_directory(outputPath + "/" + runId) ||
            !std::filesystem::exists(outputPath + "/" + runId)) {
            std::filesystem::create_directories(outputPath + "/" + runId);
        }

        //create/overwrite and open
        std::ofstream selectorLog;
        selectorLog.open(outputPath + "/" + runId + "/" + fileName + "_sel.txt", std::ofstream::trunc);

        //write selector data
        selectorLog << selector->getLog();
        //close output
        selectorLog.close();
    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

void writeAcceptorLog(boardType &board, Acceptor *&acceptor, const std::string &outputPath, const std::string &fileName,
                      const std::string &runId) {
    try {
        //mirror path for output
        if (!std::filesystem::is_directory(outputPath + "/" + runId) ||
            !std::filesystem::exists(outputPath + "/" + runId)) {
            std::filesystem::create_directories(outputPath + "/" + runId);
        }

        //create/overwrite and open
        std::ofstream acceptorLog;
        acceptorLog.open(outputPath + "/" + runId + "/" + fileName + "_acc.txt", std::ofstream::trunc);

        //write acceptor data
        acceptorLog << acceptor->getLog();
        //close output
        acceptorLog.close();

    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

void writeGeneralLog(boardType &board, Selector *&selector, Acceptor *&acceptor, string selectorMethod,
                     string acceptorMethod, std::string fileName, bool isSolved, double timeTaken,
                     double iterationsPerSecond) {

    std::string generalPath = "./program-output/experiments" + std::to_string(board.n) + "x" + std::to_string(board.n) +
                              "/" + acceptorMethod + "_" + selectorMethod + "/";
    try {
        //mirror path for output
        if (!std::filesystem::is_directory(generalPath) ||
            !std::filesystem::exists(generalPath)) {
            std::filesystem::create_directories(generalPath);
        }

        //create or open log file
        std::ofstream generalLog;
        generalLog.open(generalPath + "all_experiments_log.txt", std::fstream::app);

        //write data
        generalLog << fileName << " ";
        generalLog << isSolved << " ";
        generalLog << std::fixed << timeTaken << " ";
        generalLog << std::fixed << selector->getIterations() << " ";
        if (_isnan(iterationsPerSecond) || std::isinf(iterationsPerSecond) || selector->getIterations() < 1000)
            generalLog << 0.0 << " ";
        else
            generalLog << std::fixed << iterationsPerSecond << " ";
        generalLog << "\n";

        //close output
        generalLog.close();

    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

