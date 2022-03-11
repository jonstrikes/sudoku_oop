#include "selector.h"

Selector::Selector(){
    operators.insert(operators.end(),{
            reinterpret_cast<void (*)(boardType&)>(&swap),
            reinterpret_cast<void (*)(boardType&)>(&insert),
            reinterpret_cast<void (*)(boardType&)>(&invert),
            reinterpret_cast<void (*)(boardType&)>(&cpoExchange)
    });

    iterations = 0;
    useCounts.resize(operators.size());
    std::fill(useCounts.begin(), useCounts.end(), 0);
}

void Selector::printOperatorCounts() {
    printf("Operator counts:\n");

    printf("Swap: %d\n", useCounts[0]);
    printf("Insert: %d\n", useCounts[1]);
    printf("Invert: %d\n", useCounts[2]);
    printf("CPOExchange: %d\n", useCounts[3]);
}

int Selector::getIterations() {
    return iterations;
}
