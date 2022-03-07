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

int calcObj(boardType *board) {
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
        //board->rowObjectives[i] = rowCost;
        //board->colObjectives[i] = colCost;
        obj += rowCost + colCost;
//        std::cout << " Cost for row " << i << ": " << rowCost << std::endl;
//        std::cout << " Cost for col " << i << ": " << colCost << std::endl;
    }
    return obj;
}

// recalculates objective function for only the rows and columns changed
// returns the resulting change in objective value
int recalcObj(boardType *board) {
    //THIS CHECK MIGHT MAKE DEBUGGING NASTY (could trust the caller to perform this check??)
    if(board->moveHistory.empty())
        return 0;

    vector<int> possibleValues;
    int value = board->minCellValue;
    for (int i = 0; i < board->N; i++, value++) possibleValues.push_back(value);

    HistoryEntry lastChange = board->moveHistory.peekLast();

    int change = 0;
    //for each rowChanged
    for (const auto &rowData : lastChange.changedRowObjectives) {
        //get the changed row id and collect values along it
        int rc = rowData.first;

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
        //std::cout << "row " << rc << " prev cost " << rowData.second << " cost " << rowCost << std::endl;
        change += rowCost - board->rowObjectives[rc];//rowData.second;
        board->rowObjectives[rc] = rowCost;
    }

    //for each colChanged
    for (const auto &colData : lastChange.changedColObjectives) {
        //get the changed col id and collect values along it
        int cc = colData.first;
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
        //std::cout << "col " << cc << " prev cost " << colData.second << " cost " << colCost << std::endl;

        change += colCost - board->colObjectives[cc];//colData.second;
        board->colObjectives[cc] = colCost;
    }

    return change;
}

int calcObjInitial(boardType *board) {
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
        board->rowObjectives[i] = rowCost;
        board->colObjectives[i] = colCost;
        obj += rowCost + colCost;
//        std::cout << " Cost for row " << i << ": " << rowCost << std::endl;
//        std::cout << " Cost for col " << i << ": " << colCost << std::endl;
    }
    return obj;
}


