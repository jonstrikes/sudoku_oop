#ifndef SUDOKU_OOP_MOVE_H
#define SUDOKU_OOP_MOVE_H

#include "MoveData.h"

#include <utility>
#include <vector>
#include <map>
#include <deque>

struct HistoryEntry{
    HistoryEntry(std::vector<MoveData> changedCells,
                 std::map<int, int> &changedRowObjectives,
                 std::map<int, int> &changedColObjectives)
                 :
                 changedCells(std::move(changedCells)),
                 changedRowObjectives(changedRowObjectives),
                 changedColObjectives(changedColObjectives)
                 {}

    //an entry stores the cells changed by a LLH, and a map <row, rowObjective>
    std::vector<MoveData> changedCells;
    std::map<int, int> changedRowObjectives;
    std::map<int, int> changedColObjectives;
};

class MoveHistory{
private:
    std::deque<HistoryEntry> history;
public:
    bool empty();
    //add move to back of deque
    void recordChange(const std::vector<MoveData> &changedCells, std::vector<int> &rowObjectives, std::vector<int> &colObjectives);
    //remove last move
    HistoryEntry popChange();
    //check last record
    HistoryEntry peekLast();
};

#endif //SUDOKU_OOP_MOVE_H
