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

    board.colObjs = colObjs;
    board.rowObjs = rowObjs;

    return board;
}