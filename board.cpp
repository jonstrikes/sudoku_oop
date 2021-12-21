#include "board.h"

using std::string;

void printBoard(boardType board){
    string result;
    for (int row = 0; row < board.N; row++) {
        if (row != 0 && row % int(sqrt(board.N)) == 0) {
            for (int col = 1; col < board.N; col++) {
                result += "----";
                if (col != 0 && col % int(sqrt(board.N)) == 0)
                    result += "+";
            }

            result += "----\n";
        }
        for (int col = 0; col < board.N; col++) {
            if (col != 0 && col % int(sqrt(board.N)) == 0)
                result += "| " + cellToString(board.board[row][col]) + " ";
            else
                result += " " + cellToString(board.board[row][col]) + " ";
        }
        result += "\n";
    }
    std::cout << result << std::endl;
}

string cellToString(int value){
    std::string cell = value == -1 ? "" : std::to_string(value);
    cell.resize(2, ' ');
    return cell;
}
