#include "SudokuIO.h"

using std::string;
using std::cout;

boardType readFile(const string &fileDir) {
    std::ifstream inFile;
    inFile.open(fileDir);

    if (inFile.fail()) {
        cout << "Error: " << strerror(errno);
    }

    int n, N, minCellValue, maxCellValue;
    inFile >> n >> minCellValue;

    N = n * n;
    maxCellValue = minCellValue + N - 1;

    //initialise grid and fixed
    vector<vector<int>> grid(N, vector<int>(N));
    vector<vector<int>> fixed(N, vector<int>(N));

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

bool readCMDParams(char **input, int size, std::string &puzzleDir,
                   std::string &outputLog, std::string &solutionOutput) {
    if (size != 4 && size != 6) {
        printf("Sudoku Solver\n");
        printf("Usage: PuzzlePath OutputFile SolutionFile -acceptor -selector\n");
        printf("If either the acceptor or selector is not specified, the program defaults to Simple Random and Improve or Equal\n");
        printf("Acceptors: -oi, -ie, -sa, -gd, -la\n");
        printf("Selectors: -sr, -rd, -rp, -rpd, -g\n");
        return false;
    }

    puzzleDir = input[1];
    outputLog = input[2];
    solutionOutput = input[3];

    return true;
}

bool readCMDOptionalParams(char **input, int size, std::string &acceptorType, std::string &selectorType) {
    if (size != 6) {
        return false;
    }
    acceptorType = input[4];
    selectorType = input[5];
    return true;
}

void readAcceptorMethod(const std::string &acceptorMethod, Acceptor *&acceptor, Selector *&selector, boardType &board) {
    if (acceptorMethod == "--only-improve" || acceptorMethod == "-oi") {
        acceptor = new OnlyImprove(board);
    } else if (acceptorMethod == "--improve-or-equal" || acceptorMethod == "--improve-equal" ||
               acceptorMethod == "-ie" || acceptorMethod == "-ioe") {
        acceptor = new ImproveOrEqual(board);
    } else if (acceptorMethod == "--simulated-annealing" || acceptorMethod == "-sa") {
        acceptor = new SimulatedAnnealing(board, *selector);
    } else if (acceptorMethod == "--adaptive-iteration-limited-threshold-accepting" || acceptorMethod == "-ailta") {
        acceptor = new AdaptiveIterationLimitedThresholdAccepting(board);
    }
//    else if(acceptorMethod == "--late-acceptance" || acceptorMethod == "-la"){
//        acceptor = new LateAcceptance(board);
//    }
    else {
        acceptor = new ImproveOrEqual(board);
        printf("Acceptor %s not found, using default: Improve or Equal\n", acceptorMethod.c_str());
    }
}

void readSelectorMethod(const std::string &selectorMethod, Selector *&selector) {
    if (selectorMethod == "--simple-random" || selectorMethod == "-sr") {
        selector = new SimpleRandom();
    } else if (selectorMethod == "--random-descent" || selectorMethod == "-rd") {
        selector = new RandomDescent();
    } else if (selectorMethod == "--random-permutation" || selectorMethod == "-rp") {
        selector = new RandomPermutation();
    } else if (selectorMethod == "--random-permutation-descent" || selectorMethod == "-rpd") {
        selector = new RandomPermutationDescent();
    } else if (selectorMethod == "--greedy" || selectorMethod == "-g") {
        selector = new Greedy();
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
                     string acceptorMethod, std::string fileName, double timeTaken, double iterationsPerSecond){

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
        generalLog << std::fixed << timeTaken << " ";
        generalLog << std::fixed << selector->getIterations() << " ";
        if(_isnan(iterationsPerSecond) || std::isinf(iterationsPerSecond) || selector->getIterations() < 1000)
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

