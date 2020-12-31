#include <Arduino.h>
#include "vm.h"

void HEADER(int lex, const char seq[])
{
    P = IP >> 2;
    int i;
    int len = lex & 31;
    data[P++] = links;
    IP = P << 2;
    Serial.println();
    Serial.print(links, HEX);
    for (i = links >> 2; i < P; i++) {
        Serial.print(" ");
        Serial.print(data[i], HEX);
    }
    links = IP;
    cData[IP++] = lex;
    for (i = 0; i < len; i++) {
        cData[IP++] = seq[i];
    }
    while (IP & 3) {
        cData[IP++] = 0;
    }
    Serial.println();
    Serial.print(seq);
    Serial.print(" ");
    Serial.print(IP, HEX);
}
