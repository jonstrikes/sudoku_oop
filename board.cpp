#include "board.h"

using std::string;

void boardType::printBoard(){
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

string boardType::cellToString(int value){
    std::string cell = value == -1 ? "" : std::to_string(value);
    cell.resize(2, ' ');
    return cell;
}

bool boardType::hasChange(){
    return !this->moveHistory.empty();
}

//records a move
void boardType::rememberChange(std::vector<MoveData> &changedCells){
    moveHistory.recordChange(changedCells, rowObjectives, colObjectives);
}

//restores board and objective values to last recorded state, discarding respective entry from history
void boardType::undoChange(){
    //THIS CHECK MIGHT MAKE DEBUGGING NASTY
    if(!this->hasChange()){
        return;
    }

    HistoryEntry lastChange = moveHistory.popChange();

    //reset cells that were changed
    for (MoveData cellChanged : lastChange.changedCells) {
        board[cellChanged.getRow()][cellChanged.getCol()] = cellChanged.getVal();
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
void boardType::acceptChange(){
    if(!this->hasChange()){
        return;
    }
    while(this->hasChange()){
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

            if (missing.empty()) continue;
            //shuffle missing values
            std::shuffle(missing.begin(), missing.end(), std::mt19937(std::time(nullptr)));
            //fill in missing values
            for (int r = br * n; r < (br + 1) * n; r++) {
                for (int c = bc * n; c < (bc + 1) * n; c++) {
                    if (board[r][c] != -1) continue;
                    board[r][c] = missing.back();
                    missing.pop_back();
                }
            }
        }//end block col for
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
        for (int possibleValue = minCellValue; possibleValue < maxCellValue; possibleValue++) {
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
    if(moveHistory.empty())
        return 0;

    HistoryEntry lastChange = moveHistory.peekLast();

    int change = 0;
    //for each rowChanged
    for (const auto &rowData : lastChange.changedRowObjectives) {
        //get the id and cell values in that column
        int rc = rowData.first;

        std::vector<int> rowValues(N);
        for (int i = 0; i < N; i++) {
            rowValues.push_back(board[rc][i]);
        }
        //count number of missing cell values
        int rowCost = 0;
        for (int possibleValue = minCellValue; possibleValue < maxCellValue; possibleValue++) {
            //if row/col missing a value then increase cost
            if (std::find(rowValues.begin(), rowValues.end(), possibleValue) == rowValues.end())
                rowCost++;
        }
        //std::cout << "row " << rc << " prev cost " << rowData.second << " cost " << rowCost << std::endl;
        change += rowCost - rowObjectives[rc];//rowData.second;
        rowObjectives[rc] = rowCost;
    }

    //for each previously changed column
    for (const auto &colData : lastChange.changedColObjectives) {
        //get the id and cell values in that column
        int cc = colData.first;
        std::vector<int> colValues(N);
        for (int i = 0; i < N; i++) {
            colValues.push_back(board[i][cc]);
        }
        //count number of missing cell values
        int colCost = 0;
        for (int possibleValue = minCellValue; possibleValue < maxCellValue; possibleValue++) {
            //if some value missing then increase cost
            if (std::find(colValues.begin(), colValues.end(), possibleValue) == colValues.end())
                colCost++;
        }
        //std::cout << "col " << cc << " prev cost " << colData.second << " cost " << colCost << std::endl;

        change += colCost - colObjectives[cc];//colData.second;
        colObjectives[cc] = colCost;
    }

    return change;
}

