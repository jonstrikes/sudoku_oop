#ifndef SUDOKU_OOP_MOVERECORD_H
#define SUDOKU_OOP_MOVERECORD_H

#include <vector>
#include <cstdint>

struct MoveData {
public:
    MoveData(uint_fast8_t row, uint_fast8_t col, uint_fast8_t value,
             uint_fast8_t rowObjective, uint_fast8_t colObjective) : row(row), col(col), val(value),
                                                                     rowObjective(rowObjective),
                                                                     colObjective(colObjective) {};
    uint_fast8_t row;
    uint_fast8_t col;
    uint_fast8_t val;

    uint_fast8_t colObjective;
    uint_fast8_t rowObjective;
};

/**
 * This class is used to store a previous staet of the baord and is used for reject() operations when an acceptor does
 * not decide to keep a change
 **/
class MoveRecord {
private:
    bool isEmpty;
    std::vector<MoveData> change;

    MoveRecord(const MoveRecord&);
    MoveRecord &operator=(const MoveRecord &);
public:
    //The maximum expected changes that could be stored is bounded by
    // N when there is a sub-block that has no fixed values, where N is the length of the board
    // k where k is the smallest number of fixed values in any sub-block.
    explicit MoveRecord() : isEmpty(), change(){};

    //stores the changes made to a board
    // overwrites old record if exists
    // changes are stored to track the differences between the current state and previous state for undo operations
    void recordChange(const std::vector<MoveData> &change);

    //get the recorded state of MoveData
    std::vector<MoveData>& getChange();

    //check if no changes stored
    bool empty() const;

    //clear recorded changes
    void clear();
};

#endif //SUDOKU_OOP_MOVERECORD_H
