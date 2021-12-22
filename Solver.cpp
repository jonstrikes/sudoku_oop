#include "Solver.h"

#include <random>

using std::vector;

void fillGrid(boardType *board) {
    //iterate blocks
    for (int br = 0; br < board->n; br++) {
        for (int bc = 0; bc < board->n; bc++) {
            //initialise missing values to all possible values
            vector<int> missing;
            int value = board->minCellValue;
            for (int i = 0; i < board->N; i++, value++) missing.push_back(value);

            //collect missing values from current block
            for (int r = br * board->n; r < (br + 1) * board->n; r++) {
                for (int c = bc * board->n; c < (bc + 1) * board->n; c++) {
                    if (board->board[r][c] == -1) continue;
                    //remove encountered cell values, leaving the missing values
                    missing.erase(std::remove(missing.begin(), missing.end(), board->board[r][c]), missing.end());
                }
            }

            if (missing.empty()) continue;
            //shuffle missing values
            std::shuffle(missing.begin(), missing.end(), std::mt19937(std::time(nullptr)));
            //fill in missing values
            for (int r = br * board->n; r < (br + 1) * board->n; r++) {
                for (int c = bc * board->n; c < (bc + 1) * board->n; c++) {
                    if (board->board[r][c] != -1) continue;
                    board->board[r][c] = missing.back();
                    missing.pop_back();
                }
            }

            //make sure all missing values have been used up to fill in cells in current block
            assert(missing.empty());
        }//end block col for
    }
}

int calcObj(boardType *board, int *rowObjectives, int *colObjectives) {
    vector<int> possibleValues;
    int value = board->minCellValue;
    for (int i = 0; i < board->N; i++, value++) possibleValues.push_back(value);

    //objective function is calculated in current board like proposed in Lewis' paper
    //NOTE: maybe it would be faster to just calculate duplicate values?
    int obj = 0;
    for (int i = 0; i < board->N; i++) {
        std::vector<int> rowValues(board->N);
        std::vector<int> colValues(board->N);
        for (int j = 0; j < board->N; j++) {
            rowValues.push_back(board->board[i][j]);
            colValues.push_back(board->board[j][i]);
        }
        int rowCost = 0, colCost = 0;
        for (int possibleValue = board->minCellValue; possibleValue < board->maxCellValue; possibleValue++) {
            //if row/col missing a value then increase cost
            if (std::find(rowValues.begin(), rowValues.end(), possibleValue) == rowValues.end())
                rowCost++;
            if (std::find(colValues.begin(), colValues.end(), possibleValue) == colValues.end())
                colCost++;
        }
        rowObjectives[i] = rowCost;
        colObjectives[i] = colCost;
        obj += rowCost + colCost;
//        std::cout << " Cost for row " << i << ": " << rowCost << std::endl;
//        std::cout << " Cost for col " << i << ": " << colCost << std::endl;
    }
    return obj;
}

// recalculates objective function for only the rows and columns changed
// returns the resulting change in objective value
int recalcObj(boardType *board, int *rowObjectives, int *colObjectives,
              std::vector<std::tuple<int, int, int>> *changedCells,
              std::map<int, int> *rowPrevObjs, std::map<int, int> *colPrevObjs) {
    //MAPS SUCK. DONT USE MAPS WHEN UNDOING MORETHAN 1 MOVE, MIGHT LOSE INFORMATION

    //store row and col objective values before change
    for (std::tuple<int, int, int> cell : *changedCells) {
        int row = std::get<0>(cell), col = std::get<1>(cell);
        rowPrevObjs->insert(std::pair<int, int>(row, rowObjectives[row]));
        colPrevObjs->insert(std::pair<int, int>(col, colObjectives[col]));
    }

    vector<int> possibleValues;
    int value = board->minCellValue;
    for (int i = 0; i < board->N; i++, value++) possibleValues.push_back(value);

    int change = 0;
    //for each rowChanged
    for (const auto &pair : *rowPrevObjs) {
        //get the changed row id and collect values along it
        int rc = pair.first;
        std::vector<int> rowValues(board->N);
        for (int i = 0; i < board->N; i++) {
            rowValues.push_back(board->board[rc][i]);
        }
        //calculate cost to be count of missing values
        int rowCost = 0;
        for (int possibleValue = board->minCellValue; possibleValue < board->maxCellValue; possibleValue++) {
            //if row/col missing a value then increase cost
            if (std::find(rowValues.begin(), rowValues.end(), possibleValue) == rowValues.end())
                rowCost++;
        }
        std::cout << "row " << rc << " prev cost " << rowPrevObjs->find(rc)->second << " cost " << rowCost << std::endl;

        change += rowCost - rowObjectives[rc];
        rowObjectives[rc] = rowCost;
    }

    //for each colChanged
    for (const auto &pair : *colPrevObjs) {
        //get the changed col id and collect values along it
        int cc = pair.first;
        std::vector<int> colValues(board->N);
        for (int i = 0; i < board->N; i++) {
            colValues.push_back(board->board[i][cc]);
        }
        //calculate sum missing values
        int colCost = 0;
        for (int possibleValue = board->minCellValue; possibleValue < board->maxCellValue; possibleValue++) {
            //if row/col missing a value then increase cost
            if (std::find(colValues.begin(), colValues.end(), possibleValue) == colValues.end())
                colCost++;
        }
        std::cout << "col " << cc << " prev cost " << colPrevObjs->find(cc)->second << " cost " << colCost << std::endl;

        change += colCost - colObjectives[cc];
        colObjectives[cc] = colCost;
    }

    return change;
}

void undoMove(boardType *board, int *rowObjectives, int *colObjectives,
              std::vector<std::tuple<int, int, int>> *changedCells,
              std::map<int, int> *rowPrevObjs, std::map<int, int> *colPrevObjs) {
    //MIGHT WANT TO MAKE THIS ITERATE THE VECTOR & MAPS IN REVERSE, TO FOLLOW MOVE CHRONOLOGY IN CASE SOME METAHEURISTIC WANTS TO BACKTRACK MANY MOVES
    //VECTORS 100% NEED TO BE CLEARED ELSEWHERE OR UNDO MOVE WILL UNDO A SEQUENCE OF MOVES

    //reset cells that were changed
    for (auto const &triple : *changedCells) {
        //a tuple triple here is <row, col, value>
        board->board[std::get<0>(triple)][std::get<1>(triple)] = std::get<2>(triple);
    }
    //reset objective scores of rows to previous move
    for (auto const &pair : *rowPrevObjs) {
        //a pair is row, previous row score
        rowObjectives[std::get<0>(pair)] = std::get<1>(pair);
    }
    //reset objective scores of columns to previous move
    for (auto const &pair : *rowPrevObjs) {
        //a pair is col, previous col score
        colObjectives[std::get<0>(pair)] = std::get<1>(pair);
    }

    //board undone, clear data
    changedCells->clear();
    rowPrevObjs->clear();
    colPrevObjs->clear();
}

