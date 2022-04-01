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

bool readCMDParams(char **input, int size, std::string &puzzleDir, std::string &solutionOutPath,
                   std::string &generalLogOutPath, std::string &accLogOutPath, std::string &selLogOutPath) {
    if (size != 6 && size != 8) {
        printf("Sudoku Solver\n");
        printf("Usage: puzzleFile solutionOutFile generalLogOutFile accLogOutFile selLogOutFile -acceptor -selector\n\n");
        printf("To successfully write output to file, relative paths must be specified eg ./acceptor_logs/log1.txt \n");
        printf("Solution file is unique and gets overwritten if solutionOutFile points to an existing file \n");
        printf("General, Acceptor, Selector log files append to file without overwriting. \n");
        printf("If either the acceptor or selector is not specified, the program defaults to Simple Random and Improve or Equal\n");
        printf("Acceptance methods: -oi, -ie, -sa, -ailta\n");
        printf("Selection methods: -sr, -rd, -rp, -rpd\n");
        return false;
    }

    puzzleDir = input[1];
    solutionOutPath = input[2];
    generalLogOutPath = input[3];
    accLogOutPath = input[4];
    selLogOutPath = input[5];

    return true;
}

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType) {
    if (size != 8) {
        return false;
    }
    acceptorType = input[6];
    selectorType = input[7];
    return true;
}

void readAcceptorMethod(const std::string &acceptorMethod, nlohmann::json &specs,
                        Acceptor *&acceptor, Selector *&selector, boardType &board) {
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

void readCpParams(boardType &board, Acceptor *&acceptor, nlohmann::json &specs, CpProcessor *&cpProcessor) {
    int WORSENING_CYCLES_LIMIT, LOG_CYCLE_LIMIT;
    double CYCLE_ITERATIONS_FACTOR, WORSENING_CYCLES_FACTOR, RESET_INITIAL, RESET_MIN, RESET_MAX, RESET_ALPHA, RESET_BETA;

    try {
        WORSENING_CYCLES_LIMIT = specs["CP"]["WORSENING_CYCLES_LIMIT"];
        CYCLE_ITERATIONS_FACTOR = specs["CP"]["CYCLE_ITERATIONS_FACTOR"];
        WORSENING_CYCLES_FACTOR = specs["CP"]["WORSENING_CYCLES_FACTOR"];
        RESET_INITIAL = specs["CP"]["RESET_INITIAL"];
        RESET_MIN = specs["CP"]["RESET_MIN"];
        RESET_MAX = specs["CP"]["RESET_MAX"];
        RESET_ALPHA = specs["CP"]["RESET_ALPHA"];
        RESET_BETA = specs["CP"]["RESET_BETA"];
        LOG_CYCLE_LIMIT = specs["CP"]["LOG_CYCLE_LIMIT"];

        cpProcessor = new CpProcessor(board, acceptor, WORSENING_CYCLES_LIMIT, CYCLE_ITERATIONS_FACTOR,
                                      LOG_CYCLE_LIMIT, WORSENING_CYCLES_FACTOR,
                                      RESET_INITIAL, RESET_MIN, RESET_MAX, RESET_ALPHA, RESET_BETA);

    } catch (std::exception const &ex) {
        throw std::invalid_argument("Could not parse Constraint Programming parameters from specs.json\n");
    }
}

void readGeneralParams(boardType &board, nlohmann::json specs, double &TIME_LIMIT) {
    try {
        TIME_LIMIT = specs["Time_limits"][std::to_string(board.n)];
    } catch (std::exception const &ex) {
        throw std::invalid_argument("Could not parse general parameters from specs.json\n");
    }
}


void writeSolution(boardType &board, const string &solutionFile) {
    try {
        //create directory if doesn't exist
        string relPath = std::filesystem::path(solutionFile).relative_path().parent_path().string();

        if (!std::filesystem::is_directory(relPath) ||
            !std::filesystem::exists(relPath)) {
            std::filesystem::create_directories(relPath);
        }

        //create/overwrite and open
        std::ofstream solution;
        solution.open(std::filesystem::path(solutionFile).string(), std::ofstream::trunc);

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

void writeSelectorLog(boardType &board, Selector *&selector, const std::string &selLogOutPath,
                      const std::string &solutionOutPath) {
    try {
        //create directory if doesn't exist
        string relPath = std::filesystem::path(selLogOutPath).relative_path().parent_path().string();

        if (!std::filesystem::is_directory(relPath) ||
            !std::filesystem::exists(relPath)) {
            std::filesystem::create_directories(relPath);
        }

        //create/overwrite and open
        std::ofstream selectorLog;
        selectorLog.open(std::filesystem::path(selLogOutPath).string(), std::ofstream::app);

        //write selector data
        selectorLog << solutionOutPath << " " << selector->getLog();
        //close output
        selectorLog.close();

    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

void writeAcceptorLog(boardType &board, Acceptor *&acceptor, const std::string &accLogOutPath,
                      const std::string &solutionOutPath) {
    try {
        //create directory if doesn't exist
        string relPath = std::filesystem::path(accLogOutPath).relative_path().parent_path().string();

        if (!std::filesystem::is_directory(relPath) ||
            !std::filesystem::exists(relPath)) {
            std::filesystem::create_directories(relPath);
        }
        //create/overwrite and open
        std::ofstream acceptorLog;
        acceptorLog.open(std::filesystem::path(accLogOutPath).string(), std::ofstream::app);

        //write acceptor data
        acceptorLog << solutionOutPath << " " << acceptor->getLog();
        //close output
        acceptorLog.close();

    } catch (std::filesystem::filesystem_error const &ex) {
        printf("%s", ex.what());
    }
}

void writeGeneralLog(boardType &board, Selector *&selector, Acceptor *&acceptor, string selectorMethod,
                     string acceptorMethod, const std::string &generalLogOutPath, const std::string &solutionOutPath,
                     bool isSolved, double timeTaken, double iterationsPerSecond) {
    try {
        //create directory if doesn't exist
        string relPath = std::filesystem::path(generalLogOutPath).relative_path().parent_path().string();

        if (!std::filesystem::is_directory(relPath) ||
            !std::filesystem::exists(relPath)) {
            std::filesystem::create_directories(relPath);
        }

        //create/overwrite and open
        std::ofstream generalLog;
        generalLog.open(std::filesystem::path(generalLogOutPath).string(), std::fstream::app);

        //write data
        generalLog << solutionOutPath << " ";
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

