#ifndef SUDOKU_OOP_MOVERECORD_H
#define SUDOKU_OOP_MOVERECORD_H

#include <vector>

struct MoveData {
public:
    MoveData(int row, int col, int value, int rowObjective, int colObjective) : row(row), col(col), val(value),
                                                                                rowObjective(rowObjective),
                                                                                colObjective(colObjective) {};
    int row;
    int col;
    int val;

    int colObjective;
    int rowObjective;

//    MoveData(uint_fast8_t row, uint_fast8_t col, uint_fast8_t value, uint_fast8_t rowObjective, uint_fast8_t colObjective);
//    uint_fast8_t row;
//    uint_fast8_t col;
//    uint_fast8_t val;
//
//    uint_fast8_t colObjective;
//    uint_fast8_t rowObjective;
};

class MoveRecord {
private:
    bool isEmpty;
    std::vector<MoveData> change;
public:
    //The maximum expected changes that could be stored is bounded by
    // N when there is a sub-block that has no fixed values, where N is the length of the board
    // k where k is the smallest number of fixed values in any sub-block.
    explicit MoveRecord();

    //stores the changes made to a board
    // overwrites old record if exists
    // changes are stored to track the differences between the current state and previous state for undo operations
    void recordChange(const std::vector<MoveData> &change);

    //get the recorded state of MoveData
    std::vector<MoveData> getChange();

    //check if no changes stored
    bool empty();

    //clear recorded changes
    void clear();
};

#endif //SUDOKU_OOP_MOVERECORD_H
