#include "generalUtils.h"
#include "../SudokuIO.h"

/**
 * This pseudo-random number generation method was taken from https://stackoverflow.com/a/26237777
 * original author could not be found. The answer was submitted by user Asis and edited by ElementW
 *
 * In experiments, using this method proved faster and produced values within the same range as rand(),
 * no worse distribution than rand() was observed in experiments.
 **/
static unsigned int g_seed = readSpecification()["General"]["SEED"];
int fastrand(){
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}