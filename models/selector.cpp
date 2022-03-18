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

    timesSpent.resize(operators.size());
    std::fill(timesSpent.begin(), timesSpent.end(), 0);
}

void Selector::printOperatorCounts() {
    printf("\nOperator counts:\n");
    printf("Swap: %d\n", useCounts[0]);
    printf("Insert: %d\n", useCounts[1]);
    printf("Invert: %d\n", useCounts[2]);
    printf("CPOExchange: %d\n", useCounts[3]);

    printf("\nOperator times spent:\n");
    printf("Swap: %.5f\n", timesSpent[0]);
    printf("Insert: %.5f\n", timesSpent[1]);
    printf("Invert: %.5f\n", timesSpent[2]);
    printf("CPOExchange: %.5f\n", timesSpent[3]);

    printf("\nOperator Average times spent:\n");
    printf("Swap: %.5f\n", timesSpent[0]/useCounts[0]);
    printf("Insert: %.5f\n", timesSpent[1]/useCounts[1]);
    printf("Invert: %.5f\n", timesSpent[2]/useCounts[2]);
    printf("CPOExchange: %.5f\n", timesSpent[3]/useCounts[3]);
}

std::string Selector::getLog(){
    std::string res = std::to_string(iterations) + "\n";

    for(int i=0; i<operators.size(); i++){
        res += std::to_string(useCounts[i]) + " ";
    }
    res += "\n";

    for(int i=0; i<operators.size(); i++){
        if(useCounts[i] == 0){
            res += "0 ";
        }else{
            res += std::to_string(timesSpent[i]/useCounts[i]) + " ";
        }
    }

    return res;
}

int Selector::getIterations() {
    return iterations;
}
