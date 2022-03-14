#include "board.h"

using std::string;

boardType::boardType(int n, int N, int minCellValue, int maxCellValue,
                     vector<vector<int>> board, vector<vector<int>> fixed) :
        n(n), N(N),
        minCellValue(minCellValue), maxCellValue(maxCellValue),
        board(std::move(board)), fixed(std::move(fixed)),
        moveHistory()
{
    //initialise column and row objective containers
    rowObjectives = vector<int>(N);
    colObjectives = vector<int>(N);
}

void boardType::printBoard() {
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
}

string boardType::cellToString(int value) {
    std::string cell = value == -1 ? "" : std::to_string(value);
    cell.resize(2, ' ');
    return cell;
}

bool boardType::hasChange() {
    return !this->moveHistory.empty();
}

//records a move
void boardType::rememberChange(std::vector<MoveData> &changedCells) {
    moveHistory.recordChange(changedCells, rowObjectives, colObjectives);
}

//restores board and objective values to last recorded state, discarding respective entry from history
void boardType::undoChange() {
    if (!this->hasChange()) {
        return;
    }

    HistoryEntry lastChange = moveHistory.popChange();

    //reset cells that were changed
    for (MoveData cellChanged : lastChange.changedCells) {
        board[cellChanged.row][cellChanged.col] = cellChanged.val;
    }

    //reset objective scores of rows to previous record
    for (auto rowChange : lastChange.changedRowObjectives) {
        //a pair is row, previous row score
        rowObjectives[rowChange.first] = rowChange.second;
    }

    //reset objective scores of cols to previous record
    for (auto colChange : lastChange.changedColObjectives) {
        //a pair is col, previous col score
        colObjectives[colChange.first] = colChange.second;
    }
}

//discards all records of previous changes from history
void boardType::acceptChange() {
    if (!this->hasChange()) {
        return;
    }
    while (this->hasChange()) {
        moveHistory.popChange();
    }
}

void boardType::generateInitialSolution() {
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

            //complete block with a single empty cell
            if (missing.size() == 1) {
                for (int r = br * n; r < (br + 1) * n; r++) {
                    for (int c = bc * n; c < (bc + 1) * n; c++) {
                        //fix every cell
                        fixed[r][c] = true;
                    }
                }
            }

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
    if (moveHistory.empty())
        return 0;

    int change = 0;
    HistoryEntry lastChange = moveHistory.peekLast();

    std::vector<bool> encountered(N);
    //check rows
    for (const auto &rowData : lastChange.changedRowObjectives) {
        int rc = rowData.first;
        int rowCost = 0;

        for (int val : board[rc]) {
            if (encountered[val]) {
                rowCost++;
            }
            encountered[val] = true;
        }

        //update objectives
        change += rowCost - rowObjectives[rc];
        rowObjectives[rc] = rowCost;

        //reset work vector
        encountered.assign(encountered.size(), false);
    }

    for (const auto &colData : lastChange.changedColObjectives) {
        //get the id and cell values in that column
        int cc = colData.first;
        int colCost = 0;

        for (int row = 0; row < N; row++) {
            if (encountered[board[row][cc]]) {
                colCost++;
            }
            encountered[board[row][cc]] = true;
        }
        //update objectives
        change += colCost - colObjectives[cc];
        colObjectives[cc] = colCost;

        //reset work vector
        encountered.assign(encountered.size(), false);
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
    for (int br = 0; br < n; br++) {
        for (int bc = 0; bc < n; bc++) {
            //initialise missing values to all possible values
            vector<int> missing;
            int value = minCellValue;
            for (int i = 0; i < N; i++, value++) missing.push_back(value);

            //collect missing values from current block
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    if (fixed[r][c]) continue;
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



