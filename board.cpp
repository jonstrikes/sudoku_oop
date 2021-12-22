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

//records a move
void boardType::rememberChange(std::vector<MoveData> &changedCells){
    this->moveHistory.recordChange(changedCells, rowObjectives, colObjectives);
}

//restores board and objective values to last recorded state, discarding respective entry from history
void boardType::undoChange(){
    HistoryEntry lastChange = moveHistory.popChange();

    //reset cells that were changed
    for (MoveData cellChanged : lastChange.changedCells) {
        board[cellChanged.getRow()][cellChanged.getCol()] = cellChanged.getVal();
    }

    //reset objective scores of rows to previous move
    for (auto rowChange : lastChange.changedRowObjectives) {
        //a pair is row, previous row score
        rowObjectives[rowChange.first] = rowChange.second;
    }

    //reset objective scores of rows to previous move
    for (auto colChange : lastChange.changedColObjectives) {
        //a pair is row, previous row score
        rowObjectives[colChange.first] = colChange.second;
    }
}

//discards all records of previous changes from history
void boardType::acceptChange(){
    moveHistory.popChange();
}

