#include "MoveHistory.h"

void MoveHistory::recordChange(const std::vector<MoveData>& changedCells,
                               std::vector<int> &rowObjectives, std::vector<int> &colObjectives) {
    std::map<int, int> uniqueRowObjs;
    std::map<int, int> uniqueColObjs;

    for(MoveData entry : changedCells){
        uniqueRowObjs.insert(std::pair<int, int>(entry.getRow(), rowObjectives[entry.getRow()]));
        uniqueColObjs.insert(std::pair<int, int>(entry.getCol(), colObjectives[entry.getCol()]));
    }

    history.emplace_back(changedCells, uniqueRowObjs, uniqueColObjs);
}

HistoryEntry MoveHistory::popChange(){
    auto lastMove = history.back();
    history.pop_back();
    return lastMove;
}

HistoryEntry MoveHistory::peekLast() {
    return history.back();
}

bool MoveHistory::empty() {
    return history.empty();
}
