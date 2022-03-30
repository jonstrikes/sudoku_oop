#include "operators.h"

void invert(boardType &board) {
    int r1, c1, r2, c2;
    vector<MoveData> moveData;

    //find two unique non-fixed cells within the same block
    do {
        r1 = fastrand() % board.N;
        c1 = fastrand() % board.N;
    } while (board.fixed[r1][c1]);
    do {
        r2 = fastrand() % board.n + r1 / board.n * board.n;
        c2 = fastrand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));

    int rUp, cUp, rDown, cDown, temp;

    //determine the left and right bounds
    if (r1 > r2 || (r1 == r2 && c1 > c2)) {
        rUp = r2, cUp = c2;
        rDown = r1, cDown = c1;
    } else {
        rUp = r1, cUp = c1;
        rDown = r2, cDown = c2;
    }

    //printf("starting Left %d:%d Right %d:%d\n", rLeft, cLeft, rRight, cRight);

    //invert by swapping pairs of non-fixed cells between the left and right bounds (inclusive)
    // runs until the left and right bounds meet
    std::pair<int, int> coordinates;
    while (rUp < rDown || (rUp == rDown && cUp < cDown)) {
        if (board.fixed[rUp][cUp]) {
            coordinates = getBlockRowNeighbourCoords(true, rUp, cUp, board.n);
            rUp = coordinates.first;
            cUp = coordinates.second;
            continue;
        }
        if (board.fixed[rDown][cDown]) {
            coordinates = getBlockRowNeighbourCoords(false, rDown, cDown, board.n);
            rDown = coordinates.first;
            cDown = coordinates.second;
            continue;
        }

        //record data of the two cells before swapping
        moveData.emplace_back(rUp, cUp, board.board[rUp][cUp], board.rowObjectives[rUp],
                              board.colObjectives[cUp]);
        moveData.emplace_back(rDown, cDown, board.board[rDown][cDown], board.rowObjectives[rDown],
                              board.colObjectives[cDown]);

        //printf("swapping %d:%d and %d:%d\n", rUp, cUp, rDown, cDown);

        //apply change
        temp = board.board[rUp][cUp];
        board.board[rUp][cUp] = board.board[rDown][cDown];
        board.board[rDown][cDown] = temp;

        //find next neighbours
        coordinates = getBlockRowNeighbourCoords(true, rUp, cUp, board.n);
        rUp = coordinates.first;
        cUp = coordinates.second;
        coordinates = getBlockRowNeighbourCoords(false, rDown, cDown, board.n);
        rDown = coordinates.first;
        cDown = coordinates.second;
    }

    //board.printBoard();

    //record starting state
    board.rememberChange(moveData);
}
