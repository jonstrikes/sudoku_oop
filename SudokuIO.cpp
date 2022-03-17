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
    }
    else if(acceptorMethod == "--adaptive-iteration-limited-threshold-accepting" || acceptorMethod == "-ailta"){
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

void writeSolution(const string& inputPath, boardType &board){
    //remove extension from input file path
    string path = inputPath.substr(0, inputPath.find_last_of('.'));
    string fileName = inputPath.substr(inputPath.find_last_of('/'), inputPath.find_last_of('.'));
    //mirror path for output
    std::filesystem::create_directories("./program-output/" + inputPath);
    //create and open
    std::ofstream solution;
    solution.open ("./program-output/" + inputPath + "/" + fileName + "_sol.txt");

    //write order and min value as standard format
    solution << board.n << "\n";
    solution << board.minCellValue << "\n";

    //write out board;
    for(int i=0; i<board.N; i++){
        for(int j=0; j<board.N; j++){
            solution << board.board[i][j] << " ";
        }
        solution << "\n";
    }

    solution.close();
}