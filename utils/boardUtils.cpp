#include "boardUtils.h"

/**
 * Returns the neighbour to the left or right of a given cell within a sub block,
 * shifts to next row if no neighbour within the same row.
 *
 * rightNeighbour will go up a row when reached column boundaries and
 * !rightNeighbour will go down a row when reached column boundary.
 **/
std::pair<int, int> getBlockRowNeighbourCoords(bool rightNeighbour, int r, int c, int n) {
    int cNext, rNext;

    if (rightNeighbour) {
        if ((c + 1) % n == 0) {
            cNext = c + 1 - n;
            rNext = r + 1;
        } else cNext = c + 1, rNext = r;
    } else {
        //check if need to move to previous row
        if (c % n == 0) {
            cNext = c - 1 + n;
            rNext = r - 1;
        } else cNext = c - 1, rNext = r;
    }

    return {rNext, cNext};
}

/**
 * Returns the upper or lower neighbour of a given cell within a sub block,
 * shifts to next col if no neighbour within the same col.
 *
 * downNeighbour will choose right column when reached row boundaries and
 * !downNeighbour will choose left column when reached row boundary.
 **/
std::pair<int, int> getBlockColNeighbourCoords(bool downNeighbour, int r, int c, int n) {
    int cNext, rNext;

    if (downNeighbour) {
        if ((r + 1) % n == 0) {
            rNext = r + 1 - n;
            cNext = c + 1;
        } else rNext = r + 1, cNext = c;
    } else {
        //check if need to move to previous col
        if (r % n == 0) {
            rNext = r - 1 + n;
            cNext = c - 1;
        } else rNext = r - 1, cNext = c;
    }

    return {rNext, cNext};
}

bool areInSameBlock(int r1, int c1, int r2, int c2, int n) {
    return (r1 / n == r2 / n) && (c1 / n == c2 / n);
}

bool isWithinBounds(int r, int c, int N) {
    return (r >= 0 && r < N) && (c >= 0 && c < N);
}
