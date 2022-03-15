#include "MoveRecord.h"

void MoveRecord::recordChange(const std::vector<MoveData> &aChange) {
    //could try to just feed the MoveData arguments for each change?
    change = aChange;
    isEmpty = false;
}

std::vector<MoveData>& MoveRecord::getChange() {
    return change;
}

void MoveRecord::clear() {
    change.clear();
    isEmpty = true;
}

bool MoveRecord::empty() const {
    return isEmpty;
}
