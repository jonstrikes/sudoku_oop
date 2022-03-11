#include "SudokuIO.h"

using std::string;
using std::cout;

boardType readFile(const string& fileDir){
    std::ifstream inFile;
    inFile.open(fileDir);

    if (inFile.fail()){
        cout << "Error: " << strerror(errno);
    }

    int n, N, minCellValue, maxCellValue;
    inFile >> n >> minCellValue;

    N = n*n;
    maxCellValue = minCellValue + N;

    //initialise grid and fixed
    auto grid = vector<vector<int>> (N, vector<int>(N));
    auto fixed = vector<vector<int>> (N, vector<int>(N));

    //initialise column and row objective containers
    auto colObjs = vector<int>(N);
    auto rowObjs = vector<int>(N);

    //all benchmark files contain one puzzle
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int current_cell;
            inFile >> current_cell;
            grid[row][col] = current_cell;
            fixed[row][col] = current_cell != -1;
        }
    }

    boardType board{};
    board.n = n;
    board.N = N;

    board.minCellValue = minCellValue;
    board.maxCellValue = maxCellValue;

    board.board = grid;
    board.fixed = fixed;

    board.colObjectives = colObjs;
    board.rowObjectives = rowObjs;

    //initialise boards move history
    board.moveHistory = MoveHistory();

    return board;
}

bool readCMDParams(char **input, int size, std::string& puzzleDir, std::string& outputLog, std::string& solutionOutput){
    if(size != 4 && size != 6){
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

bool readCMDOptionalParams(char **input, int size, std::string& acceptorType, std::string& selectorType){
    if(size != 6){
        return false;
    }
    acceptorType = input[4];
    selectorType = input[5];
    return true;
}

void readAcceptorMethod(const std::string& acceptorMethod, Acceptor *&acceptor, Selector *&selector, boardType &board){
    if(acceptorMethod == "--only-improve" || acceptorMethod == "-oi"){
        acceptor = new OnlyImprove(board);
    }
    else if(acceptorMethod == "--improve-or-equal" || acceptorMethod == "-ie"){
        acceptor = new ImproveOrEqual(board);
    }
    else if(acceptorMethod == "--simulated-annealing" || acceptorMethod == "-sa"){
        acceptor = new SimulatedAnnealing(board, *selector);
    }
//    else if(acceptorMethod == "--great-deluge" || acceptorMethod == "-gd"){
//        acceptor = new GreatDeluge(board);
//    }
//    else if(acceptorMethod == "--late-acceptance" || acceptorMethod == "-la"){
//        acceptor = new LateAcceptance(board);
//    }
    else{
        acceptor = new ImproveOrEqual(board);
        printf("Acceptor %s not found, using default: Improve or Equal\n", acceptorMethod.c_str());
    }
}

void readSelectorMethod(const std::string& selectorMethod, Selector *&selector){
    if(selectorMethod == "--simple-random" || selectorMethod == "-sr"){
        selector = new SimpleRandom();
    }
    else if(selectorMethod == "--random-descent" || selectorMethod == "-rd"){
        selector = new RandomDescent();
    }
    else if(selectorMethod == "--random-permutation" || selectorMethod == "-rp"){
        selector = new RandomPermutation();
    }
    else if(selectorMethod == "--random-permutation-descent" || selectorMethod == "-rpd"){
        selector = new RandomPermutationDescent();
    }
    else if(selectorMethod == "--greedy" || selectorMethod == "-g"){
        selector = new Greedy();
    }
    else{
        selector = new SimpleRandom();
        printf("Selector %s not found, using default: Simple Random\n", selectorMethod.c_str());
    }
}