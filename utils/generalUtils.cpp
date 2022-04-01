#include "generalUtils.h"
#include "../SudokuIO.h"
static unsigned int g_seed = readSpecification()["General"]["SEED"];
int fastrand(){
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}