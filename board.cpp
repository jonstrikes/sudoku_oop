#include "board.h"

using std::string;

boardType::boardType(int n, int N, int minCellValue, int maxCellValue,
                     vector<vector<int>> board, vector<vector<bool>> fixed) :
        n(n), N(N),
        minCellValue(minCellValue), maxCellValue(maxCellValue),
        board(std::move(board)), fixed(std::move(fixed)),
        moveRecord(), encounteredAlongRow(N), encounteredAlongCol(N)
{
    //initialise column and row objective containers
    rowObjectives = vector<uint_fast8_t>(N);
    colObjectives = vector<uint_fast8_t>(N);
}

string boardType::printBoard() {
    string result;
    for (int row = 0; row < N; row++) {
        if (row != 0 && row % int(sqrt(N)) == 0) {
            for (int col = 1; col < N; col++) {
                result += "----";
                if (col != 0 && col % int(sqrt(N)) == 0)
                    result += "+";
            }

            result += "----\n";
        }
        for (int col = 0; col < N; col++) {
            if (col != 0 && col % int(sqrt(N)) == 0)
                result += "| " + cellToString(board[row][col]) + " ";
            else
                result += " " + cellToString(board[row][col]) + " ";
        }
        result += "\n";
    }
    std::cout << result << std::endl;

    return result;
}

string boardType::cellToString(int value) {
    std::string cell = value == -1 ? "" : std::to_string(value);
    cell.resize(2, ' ');
    return cell;
}

bool boardType::hasChange() {
    return !this->moveRecord.empty();
}

//records a move
void boardType::rememberChange(std::vector<MoveData> &changedCells) {
    if(changedCells.empty())
        return;

    moveRecord.recordChange(changedCells);
}

//restores board and objective values to last recorded state, discarding respective entry from history
void boardType::undoChange() {
    if (!this->hasChange()) {
        return;
    }

    for(MoveData cell : moveRecord.getChange()){
        board[cell.row][cell.col] = cell.val;
        rowObjectives[cell.row] = cell.rowObjective;
        colObjectives[cell.col] = cell.colObjective;
    }

    moveRecord.clear();
}

//discards all records of previous changes from history
void boardType::acceptChange() {
    if (!this->hasChange()) {
        return;
    }

    moveRecord.clear();
}

void boardType::generateSolution() {
    for (int br = 0; br < n; br++) {
        for (int bc = 0; bc < n; bc++) {
            //initialise missing values to all possible values
            vector<int> missing;
            int value = minCellValue;
            for (int i = 0; i < N; i++, value++) missing.push_back(value);

            //collect missing values from current block
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    if (board[r][c] == -1) continue;
                    //remove encountered cell values, leaving the missing values
                    missing.erase(std::remove(missing.begin(), missing.end(), board[r][c]), missing.end());
                }
            }

            //do nothing if block has no empty cells
            if (missing.empty())
                continue;

            //shuffle missing values
            std::shuffle(missing.begin(), missing.end(), std::mt19937(std::time(nullptr)));
            //fill in missing values
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    //skip non-empty cells
                    if (board[r][c] != -1)
                        continue;

                    board[r][c] = missing.back();
                    missing.pop_back();
                }
            }

        }
    }
}

int boardType::calculateObjective() {
    vector<int> possibleValues;
    int value = minCellValue;
    for (int i = 0; i < N; i++, value++) possibleValues.push_back(value);

    //objective function is calculated in current board like proposed in Lewis' paper
    //NOTE: maybe it would be faster to just calculate duplicate values?
    int obj = 0;
    for (int i = 0; i < N; i++) {
        std::vector<int> rowValues(N);
        std::vector<int> colValues(N);
        for (int j = 0; j < N; j++) {
            rowValues.push_back(board[i][j]);
            colValues.push_back(board[j][i]);
        }
        int rowCost = 0, colCost = 0;
        for (int possibleValue = minCellValue; possibleValue <= maxCellValue; possibleValue++) {
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

int boardType::updateObjective() {
    if (moveRecord.empty())
        return 0;

    int change = 0;

    //this may calculate rows and cols multiple times
    for(MoveData cell : moveRecord.getChange()){
        uint_fast8_t rowCost = 0, colCost = 0;

        for(int i=0; i<N; i++){
            if(encounteredAlongRow[board[cell.row][i]])
                rowCost++;
            encounteredAlongRow[board[cell.row][i]] = true;

            if(encounteredAlongCol[board[i][cell.col]])
                colCost++;
            encounteredAlongCol[board[i][cell.col]] = true;
        }

        //update objective scores
        change += rowCost - rowObjectives[cell.row];
        rowObjectives[cell.row] = rowCost;

        change += colCost - colObjectives[cell.col];
        colObjectives[cell.col] = colCost;

        //reset work vectors
        encounteredAlongRow.assign(N, false);
        encounteredAlongCol.assign(N, false);
    }

    return change;
}

bool boardType::verifySolved() {
    bool isCorrect = true;

    //check rows
    for (const std::vector<int> &row : board) {
        std::vector<bool> encountered(N);

        for (int val : row) {
            if (encountered[val]) {
                isCorrect = false;
                std::cout << "! Duplicate row value found: " << val << std::endl;
            }
            encountered[val] = true;
        }

        //reset work vector
        encountered.assign(encountered.size(), false);
    }

    //check columns
    for (int col = 0; col < N; col++) {
        std::vector<bool> encountered(N);
        for (int row = 0; row < N; row++) {
            if (encountered[board[row][col]]) {
                isCorrect = false;
                std::cout << "! Duplicate col value found: " << board[row][col] << std::endl;
            }
            encountered[board[row][col]] = true;
        }

        //reset work vector
        encountered.assign(encountered.size(), false);
    }

    //check blocks
    for (int br = 0; br < n; br++) {
        for (int bc = 0; bc < n; bc++) {
            std::vector<bool> encountered(N);
            //iterate values of each block
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    if (encountered[board[r][c]]) {
                        isCorrect = false;
                        std::cout << "! Duplicate block value found: " << board[r][c] << std::endl;
                    }
                    encountered[board[r][c]] = true;
                }
            }
            //reset work vector
            encountered.assign(encountered.size(), false);
        }
    }

    return isCorrect;
}

void boardType::randomiseExistingSolution() {
    moveRecord.clear();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if(!fixed[i][j]){
                board[i][j] = -1;
            }
        }
    }
    printBoard();

    generateSolution();
}

std::set<std::pair<uint_fast8_t , uint_fast8_t>> boardType::getConflictingCells() {
    std::set<std::pair<uint_fast8_t, uint_fast8_t>> conflictingCells;

    std::vector<bool> isDuplicateInRow(N);
    std::vector<bool> isDuplicateInCol(N);

    for(int i=0; i<N; i++){
        if(rowObjectives[i] == 0)
            goto check_columns;

        //mark duplicate values in rows
        for(int j=0; j<N; j++) {
            if(encounteredAlongRow[board[i][j]])
                isDuplicateInRow[board[i][j]] = true;
            encounteredAlongRow[board[i][j]] = true;
        }

        //collect conflicting cells along rows
        for(int j=0; j<N; j++) {
            if(isDuplicateInRow[board[i][j]] && !fixed[i][j]){
                conflictingCells.insert(std::make_pair(i,j));
            }
        }

        //reset work vectors for rows
        encounteredAlongRow.assign(N, false);
        isDuplicateInRow.assign(N, false);

        //now check columns
        check_columns:
        if(colObjectives[i] == 0)
            continue;

        //mark duplicate values in cols
        for(int j=0; j<N; j++) {
            if(encounteredAlongCol[board[j][i]])
                isDuplicateInCol[board[j][i]] = true;
            encounteredAlongCol[board[j][i]] = true;
        }

        //collect conflicting cells along cols
        for(int j=0; j<N; j++) {
            if(isDuplicateInCol[board[j][i]] && !fixed[j][i]){
                conflictingCells.insert(std::make_pair(j,i));
            }
        }

        //reset work vectors for cols
        encounteredAlongCol.assign(N, false);
        isDuplicateInCol.assign(N, false);
    }

    return conflictingCells;
}

void boardType::fixBlocksWithSingleCellMissing() {
    for (int br = 0; br < n; br++) {
        for (int bc = 0; bc < n; bc++) {
            //initialise missing values to all possible values
            int unfixedCount = 0;
            vector<int> missing;

            int value = minCellValue;
            for (int i = 0; i < N; i++, value++) missing.push_back(value);

            //collect missing values from current block
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    if (board[r][c] == -1) {
                        unfixedCount++;
                        continue;
                    }
                    else if(!fixed[r][c]){
                        unfixedCount++;
                    }
                    //remove encountered cell values, leaving the missing values
                    missing.erase(std::remove(missing.begin(), missing.end(), board[r][c]), missing.end());
                }
            }

            if (unfixedCount == 1) {
                for (int r = br * n; r < (br + 1) * n; r++) {
                    for (int c = bc * n; c < (bc + 1) * n; c++) {
                        if(!fixed[r][c]) {
                            board[r][c] = missing.back();
                            fixed[r][c] = true;
                        }
                    }
                }
            }
        }
    }

}



