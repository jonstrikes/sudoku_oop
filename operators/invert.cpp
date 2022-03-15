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

    int rLeft, cLeft, rRight, cRight, temp;

    //determine the left and right bounds
    if (r1 > r2 || r1 == r2 && c1 > c2) {
        rLeft = r2, cLeft = c2;
        rRight = r1, cRight = c1;
    } else {
        rLeft = r1, cLeft = c1;
        rRight = r2, cRight = c2;
    }

    //invert by swapping pairs of non-fixed cells between the left and right bounds (inclusive)
    // runs until the left and right bounds meet
    std::pair<int, int> coordinates;
    while (rLeft < rRight || (rLeft == rRight && cLeft < cRight)) {
        if (board.fixed[rLeft][cLeft]) {
            coordinates = getNeighbourCoords(true, rLeft, cLeft, board.n);
            rLeft = coordinates.first;
            cLeft = coordinates.second;
            continue;
        }
        if (board.fixed[rRight][cRight]) {
            coordinates = getNeighbourCoords(false, rRight, cRight, board.n);
            rRight = coordinates.first;
            cRight = coordinates.second;
            continue;
        }

        //record data of the two cells before swapping
        moveData.emplace_back(rLeft, cLeft, board.board[rLeft][cLeft], board.rowObjectives[rLeft],
                              board.colObjectives[cLeft]);
        moveData.emplace_back(rRight, cRight, board.board[rRight][cRight], board.rowObjectives[rRight],
                              board.colObjectives[cRight]);

        //apply change
        temp = board.board[rLeft][cLeft];
        board.board[rLeft][cLeft] = board.board[rRight][cRight];
        board.board[rRight][cRight] = temp;

        //find next neighbours
        coordinates = getNeighbourCoords(true, rLeft, cLeft, board.n);
        rLeft = coordinates.first;
        cLeft = coordinates.second;
        coordinates = getNeighbourCoords(false, rRight, cRight, board.n);
        rRight = coordinates.first;
        cRight = coordinates.second;
    }

    //record starting state
    board.rememberChange(moveData);
}
