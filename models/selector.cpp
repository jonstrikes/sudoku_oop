#include "selector.h"

Selector::Selector(){
    operators.insert(operators.end(),{
            reinterpret_cast<void (*)(boardType&)>(&swap),
            reinterpret_cast<void (*)(boardType&)>(&insert),
            reinterpret_cast<void (*)(boardType&)>(&invert),
            reinterpret_cast<void (*)(boardType&)>(&cpoExchange)
    });
}
