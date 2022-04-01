#include "selector.h"

Selector::Selector(){
    operators.insert(operators.end(),{
            reinterpret_cast<void (*)(boardType&)>(&swap),
            reinterpret_cast<void (*)(boardType&)>(&insert),
            reinterpret_cast<void (*)(boardType&)>(&insertSym),
            reinterpret_cast<void (*)(boardType&)>(&swapConflicting),
            reinterpret_cast<void (*)(boardType&)>(&invert),
            reinterpret_cast<void (*)(boardType&)>(&invertSym),
            reinterpret_cast<void (*)(boardType&)>(&cpoExchange)
    });

    iterations = 0;
    useCounts.resize(operators.size());
    std::fill(useCounts.begin(), useCounts.end(), 0);

    timesSpent.resize(operators.size());
    std::fill(timesSpent.begin(), timesSpent.end(), 0);
}

void Selector::applyOperator(boardType &board, int operatorId) {
    iterations++;
    clock_t t = clock();
    (*operators[operatorId])(board);
    timesSpent[operatorId] += (double)(clock() - t);

    useCounts[operatorId]++;
}

int Selector::getIterations() {
    return iterations;
}

void Selector::printLog() {
    printf("\n=========== Selector ===========\n");

    printf("Operator counts:\n");
    printf("%-20.15s %d\n", "Swap:", useCounts[0]);
    printf("%-20.15s %d\n", "Insert:", useCounts[1]);
    printf("%-20.15s %d\n", "InsertSym:", useCounts[2]);
    printf("%-20.15s %d\n", "SwapConf:", useCounts[3]);
    printf("%-20.15s %d\n", "Invert:", useCounts[4]);
    printf("%-20.15s %d\n", "InvertSym:", useCounts[5]);
    printf("%-20.15s %d\n", "CPOExchange:", useCounts[6]);

    printf("\nOperator times spent:\n");
    printf("%-20.15s %.5es\n", "Swap:", timesSpent[0] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "Insert:", timesSpent[1] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "InsertSym:", timesSpent[2] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "SwapConf:", timesSpent[3] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "Invert:", timesSpent[4] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "InvertSym:", timesSpent[5] / CLOCKS_PER_SEC);
    printf("%-20.15s %.5es\n", "CPOExchange:", timesSpent[6] / CLOCKS_PER_SEC);

    printf("\nOperator Average times spent:\n");
    printf("%-20.15s %.5es\n", "Swap:", timesSpent[0]/ (useCounts[0] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "Insert:", timesSpent[1]/ (useCounts[1] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "InsertSym:", timesSpent[2]/ (useCounts[2] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "SwapConf:", timesSpent[3]/ (useCounts[3] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "Invert:", timesSpent[4]/ (useCounts[4] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n", "InvertSym:", timesSpent[5]/ (useCounts[5] * CLOCKS_PER_SEC));
    printf("%-20.15s %.5es\n\n", "CPOExchange:", timesSpent[6]/ (useCounts[6] * CLOCKS_PER_SEC));
}

std::string Selector::getLog(){
    std::string res = std::to_string(iterations) + " " + std::to_string(operators.size()) + " ";

    for(int i=0; i<operators.size(); i++){
        res += std::to_string(useCounts[i]) + " ";
    }

    for(int i=0; i<operators.size(); i++){
        res += std::to_string(timesSpent[i]) + " ";
    }

    for(int i=0; i<operators.size(); i++){
        if(useCounts[i] == 0){
            res += "0 ";
        }else{
            res += std::to_string(timesSpent[i]/useCounts[i]) + " ";
        }
    }

    return res + "\n";
}
