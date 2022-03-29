#include "cpProcedure.h"

std::pair<int, int> getImplicitValueIfExists(boardType &board, std::vector<std::set<uint_fast8_t>> listsOfSequence);
std::set<uint_fast8_t> getPossibleValues(boardType &board, int row, int col);


void cpProcedure(boardType &board, float resetProb) {
//    std::cout << "before reseting conflicting cells:" << std::endl;
//    board.printBoard();

    //reset conflicting cells
    std::set<std::pair<uint_fast8_t, uint_fast8_t>> conflictingCells = board.getConflictingCells();

    for (auto cell : conflictingCells) {
        board.board[cell.first][cell.second] = -1;
    }

//    std::cout << "after reseting conflicting cells:" << std::endl;
//    board.printBoard();

    //reset additional unfixed cells specified by reset
    for (int i = 0; i < board.N; i++) {
        for (int j = 0; j < board.N; j++) {
            if(!board.fixed[i][j]) {
                if (resetProb > ((double)fastrand() / RAND_MAX)) {
                    board.board[i][j] = -1;
                }
            }
        }
    }

    //constraint programming stage
    //auto fixedOrMarked(board.fixed);


    bool hasChange = true;

    while (hasChange) {
        restart:
        std::vector<std::vector<std::set<uint_fast8_t>>> boardLists;

        for (int i = 0; i < board.N; i++) {
            std::vector<std::set<uint_fast8_t>> rowLists;

            for (int j = 0; j < board.N; j++) {
                if (!board.fixed[i][j]) {
                    //get all possible values for current cell
                    auto possibleValues = getPossibleValues(board, i, j);

                    //if only one value possible, fill cell with implicit value and restart
                    if (possibleValues.size() == 1) {
                        board.fixed[i][j] = true;
                        board.board[i][j] = *possibleValues.begin();
                        goto restart;
                    }

                    rowLists.emplace_back(possibleValues);
                } else {
                    std::set<uint_fast8_t> empty;
                    rowLists.emplace_back(empty);
                }
            }

            boardLists.emplace_back(rowLists);
        }

        //rows
        for (int i = 0; i < board.N; i++) {
            auto res = getImplicitValueIfExists(board, boardLists[i]);

            if(res.first != -1 && res.second != -1){
                board.board[i][res.first] = res.second;
                board.fixed[i][res.first] = true;
                goto restart;
            }
        }

        //cols
        std::vector<std::set<uint_fast8_t>> possibleValueLists;
        for (int i = 0; i < board.N; i++) {
            for(int j = 0; j < board.N; j++){
                possibleValueLists.push_back(boardLists[j][i]);
            }

            auto res = getImplicitValueIfExists(board, possibleValueLists);

            if(res.first != -1 && res.second != -1){
                board.board[res.first][i] = res.second;
                board.fixed[res.first][i] = true;
                goto restart;
            }

            possibleValueLists.clear();
        }

        //blocks
        for (int br=0; br<board.n; br++) {
            for (int bc=0; bc<board.n; bc++) {

                for (int r = br * board.n; r < (br + 1) * board.n; r++) {
                    for (int c = bc * board.n; c < (bc + 1) * board.n; c++) {
                        possibleValueLists.push_back(boardLists[r][c]);
                    }
                }

                auto res = getImplicitValueIfExists(board, possibleValueLists);

                if(res.first != -1 && res.second != -1){
                    int row = (br * board.n) + (res.first / board.n);
                    int col = (bc * board.n) + (res.first % board.n);

                    board.board[row][col] = res.second;
                    board.fixed[row][col] = true;
                    goto restart;
                }

                possibleValueLists.clear();
            }
        }

        hasChange = false;
    }
}

std::pair<int, int> getImplicitValueIfExists(boardType &board, std::vector<std::set<uint_fast8_t>> listsOfSequence) {

    std::vector<bool> encountered(board.N);

    //try each value
    for (int possibleValue = board.minCellValue; possibleValue <= board.maxCellValue; possibleValue++) {
        int lastEncounter = -1;

        //iterate over sequence
        for (int i = 0; i < board.N; i++) {
            if (listsOfSequence[i].find(possibleValue) != listsOfSequence[i].end()) {
                //if current value is encountered more than once try next
                if (encountered[possibleValue]) {
                    goto endOfIteration;
//                    encountered.assign(boardSequence.size(), false);
//                    break;
                }
                //mark last encounter in-case it is the only one in the sequence
                lastEncounter = i;
                encountered[possibleValue] = true;
            }
        }

        if (std::count(encountered.begin(), encountered.end(), true) == 1) {
            //fill value
//            boardSequence[lastEncounter] = possibleValue;
//            board.fixed[lastEncounter] = true;
//            return true;
            return std::make_pair(lastEncounter, possibleValue);
        }

        endOfIteration:
        encountered.assign(board.N, false);
    }

    return std::make_pair(-1, -1);
}

std::set<uint_fast8_t> getPossibleValues(boardType &board, int row, int col) {
    std::vector<bool> encountered(board.N);

    //find values that appear as fixed or marked along col and row
    for (int i = 0; i < board.N; i++) {
        if (board.fixed[row][i])
            encountered[board.board[row][i]] = true;
        if (board.fixed[i][col])
            encountered[board.board[i][col]] = true;
    }

    //find values that appear as fixed or marked in block
    int blockX = row / board.n * board.n;
    int blockY = col / board.n * board.n;

    for (int i = blockX; i < blockX + board.n; i++) {
        for (int j = blockY; j < blockY + board.n; j++) {
            if (board.fixed[i][j])
                encountered[board.board[i][j]] = true;
        }
    }

    std::set<uint_fast8_t> notEncountered;
    for (int i = board.minCellValue; i <= board.maxCellValue; i++) {
        if (!encountered[i]) {
            notEncountered.insert(i);
        }
    }

    return notEncountered;
}
