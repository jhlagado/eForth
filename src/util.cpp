#include <Arduino.h>

#include "primitives.h"
#include "vm.h"

unsigned char bytecode;
int counter = 0;

void evaluate()
{
    while (true) {
        if (counter++ > 10000) {
            delay(1);
            counter = 0;
        }
        bytecode = (unsigned char)cData[P++];
        if (bytecode) {
            primitives[bytecode]();
        } else {
            break;
        }
    } // break on NOP
}
