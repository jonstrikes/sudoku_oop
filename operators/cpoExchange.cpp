#include "operators.h"

void cpoExchange(boardType &board) {
    int r, c;
    vector<MoveData> moveData;

    //set a unique non-fixed cell as the center point
    do {
        r = fastrand() % board.N;
        c = fastrand() % board.N;
    } while (board.fixed[r][c]);

    std::pair<int, int> left = getBlockRowNeighbourCoords(false, r, c, board.n);
    std::pair<int, int> right = getBlockRowNeighbourCoords(true, r, c, board.n);
    //swap pairs of non-fixed values within the same block around the center point
    // exchanges pairs until fixed value is bumped into or reached the end of sub-block
    while (true) {
        if (!isWithinBounds(left.first, left.second, board.N) || !isWithinBounds(right.first, right.second, board.N))
            break;

        if (!areInSameBlock(left.first, left.second, right.first, right.second, board.n))
            break;

        if (board.fixed[left.first][left.second] || board.fixed[right.first][right.second])
            break;

        //record data of the cell pair before exchange
        moveData.emplace_back(left.first, left.second, board.board[left.first][left.second],
                              board.rowObjectives[left.first], board.colObjectives[left.second]);
        moveData.emplace_back(right.first, right.second, board.board[right.first][right.second],
                              board.rowObjectives[right.first], board.colObjectives[right.second]);

        //swap neighbours
        int temp = board.board[left.first][left.second];
        board.board[left.first][left.second] = board.board[right.first][right.second];
        board.board[right.first][right.second] = temp;

        //get further neighbours
        left = getBlockRowNeighbourCoords(false, left.first, left.second, board.n);
        right = getBlockRowNeighbourCoords(true, right.first, right.second, board.n);
    }

    //record previous state
    board.rememberChange(moveData);
}

