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

