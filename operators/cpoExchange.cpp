#include "../board.h"
#include "../utils/boardUtils.h"

void cpoExchange(boardType &board) {
    std::cout << "CPOExchange called" << std::endl;

    int r, c;
    vector<MoveData> moveData;

    do {
        r = rand() % board.N;
        c = rand() % board.N;
    } while (board.fixed[r][c]);

    //std::cout << "start point: " << r << ":" << c << std::endl;

    std::pair<int, int>left = getNeighbourCoords(false, r, c, board.n);
    std::pair<int, int>right = getNeighbourCoords(true, r, c, board.n);

    while(true){
        if(!isWithinBounds(left.first, left.second, board.N) || !isWithinBounds(right.first, right.second, board.N))
            break;

        if(!areInSameBlock(left.first, left.second, right.first, right.second, board.n))
            break;

        if(board.fixed[left.first][left.second] || board.fixed[right.first][right.second])
            break;

        //std::cout << "neighbours left " << left.first << ":" << left.second << " right " << right.first << ":" << right.second <<  std::endl;

        moveData.emplace_back(left.first, left.second, board.board[left.first][left.second]);
        moveData.emplace_back(right.first, right.second, board.board[right.first][right.second]);

        //swap neighbours
        int temp = board.board[left.first][left.second];
        board.board[left.first][left.second] = board.board[right.first][right.second];
        board.board[right.first][right.second] = temp;

        //get further neighbours
        left = getNeighbourCoords(false, left.first, left.second, board.n);
        right = getNeighbourCoords(true, right.first, right.second, board.n);
    }

    if(!moveData.empty()) {
        board.rememberChange(moveData);
    }
}

