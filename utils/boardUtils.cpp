#include "boardUtils.h"

std::pair<int, int> getNeighbourCoords(bool rightNeighbour, int r, int c, int n){
    int cNext, rNext;

    if(rightNeighbour){
        if((c+1) % n == 0){
            cNext = c+1 - n;
            rNext = r+1;
        }
        else cNext = c+1, rNext=r;
    }
    else{
        //check if need to move to previous row
        if(c % n == 0){
            cNext = c-1 + n;
            rNext = r-1;
        }
        else cNext=c-1, rNext=r;
    }

    return {rNext, cNext};
}

bool areInSameBlock(int r1, int c1, int r2, int c2, int n){
    return (r1/n == r2/n) && (c1/n == c2/n);
}

bool isWithinBounds(int r, int c, int N){
    return (r >= 0 && r < N) && (c >= 0 && c < N);
}
