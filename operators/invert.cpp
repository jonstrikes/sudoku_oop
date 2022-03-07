#include "../board.h"
#include "../utils/boardUtils.h"

void invert(boardType &board){
    int r1, c1, r2, c2;
    vector<MoveData> moveData;

//    std::cout << "Invert called" << std::endl;

    do {
        r1 = rand() % board.N;
        c1 = rand() % board.N;
    } while (board.fixed[r1][c1]);

    do {
        r2 = rand() % board.n + r1 / board.n * board.n;
        c2 = rand() % board.n + c1 / board.n * board.n;
    } while (board.fixed[r2][c2] || (r1 == r2 && c1 == c2));

    int rLeft, cLeft, rRight, cRight, temp;

    if(r1 > r2 || r1 == r2 && c1 > c2){
        rLeft = r2, cLeft = c2;
        rRight = r1, cRight = c1;
    }
    else{
        rLeft = r1, cLeft = c1;
        rRight = r2, cRight = c2;
    }

    std::pair<int, int> coordinates;
    while(rLeft < rRight || ( rLeft == rRight && cLeft < cRight)){
        if(board.fixed[rLeft][cLeft]){
            coordinates = getNeighbourCoords(true, rLeft, cLeft, board.n);
            rLeft = coordinates.first;
            cLeft = coordinates.second;
            continue;
        }
        if(board.fixed[rRight][cRight]){
            coordinates = getNeighbourCoords(false, rRight, cRight, board.n);
            rRight = coordinates.first;
            cRight = coordinates.second;
            continue;
        }


        moveData.emplace_back(rLeft, cLeft, board.board[rLeft][cLeft]);
        moveData.emplace_back(rRight, cRight, board.board[rRight][cRight]);

        temp = board.board[rLeft][cLeft];
        board.board[rLeft][cLeft] = board.board[rRight][cRight];
        board.board[rRight][cRight] = temp;

        coordinates = getNeighbourCoords(true, rLeft, cLeft, board.n);
        rLeft = coordinates.first;
        cLeft = coordinates.second;

        coordinates = getNeighbourCoords(false, rRight, cRight, board.n);
        rRight = coordinates.first;
        cRight = coordinates.second;
    }

    board.rememberChange(moveData);
}
