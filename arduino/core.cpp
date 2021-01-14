#include <Arduino.h>
#include "vm.h"

int BRAN = 0, QBRAN = 0, DONXT = 0, DOTQP = 0, STRQP = 0, TOR = 0, ABORQP = 0;

int IMEDD = 0x80;
int COMPO = 0x40;

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

int CODE(int len, ...)
{
    int addr = IP;
    int s;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        s = va_arg(argList, int);
        cData[IP++] = s;
        Serial.print(" ");
        Serial.print(s, HEX);
    }
    va_end(argList);
    return addr;
}
int COLON(int len, ...)
{
    int addr = IP;
    P = IP >> 2;
    data[P++] = 6; // dolist
    va_list argList;
    va_start(argList, len);
    Serial.println();
    Serial.print(addr, HEX);
    Serial.print(" ");
    Serial.print(6, HEX);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
    return addr;
}
int LABEL(int len, ...)
{
    int addr = IP;
    P = IP >> 2;
    va_list argList;
    va_start(argList, len);
    Serial.println();
    Serial.print(addr, HEX);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
    return addr;
}
void BEGIN(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" BEGIN ");
    pushR = P;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void AGAIN(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" AGAIN ");
    data[P++] = BRAN;
    data[P++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void UNTIL(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" UNTIL ");
    data[P++] = QBRAN;
    data[P++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void WHILE(int len, ...)
{
    P = IP >> 2;
    int k;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" WHILE ");
    data[P++] = QBRAN;
    data[P++] = 0;
    k = popR;
    pushR = (P - 1);
    pushR = k;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void REPEAT(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" REPEAT ");
    data[P++] = BRAN;
    data[P++] = popR << 2;
    data[popR] = P << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void IF(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" IF ");
    data[P++] = QBRAN;
    pushR = P;
    data[P++] = 0;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void ELSE(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" ELSE ");
    data[P++] = BRAN;
    data[P++] = 0;
    data[popR] = P << 2;
    pushR = P - 1;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void THEN(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" THEN ");
    data[popR] = P << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void FOR(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" FOR ");
    data[P++] = TOR;
    pushR = P;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void NEXT(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" NEXT ");
    data[P++] = DONXT;
    data[P++] = popR << 2;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void AFT(int len, ...)
{
    P = IP >> 2;
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" AFT ");
    data[P++] = BRAN;
    data[P++] = 0;
    // int k = popR;
    pushR = P;
    pushR = P - 1;
    va_list argList;
    va_start(argList, len);
    for (; len; len--) {
        int j = va_arg(argList, int);
        data[P++] = j;
        Serial.print(" ");
        Serial.print(j, HEX);
    }
    IP = P << 2;
    va_end(argList);
}
void DOTQ(const char seq[])
{
    P = IP >> 2;
    int i;
    int len = strlen(seq);
    data[P++] = DOTQP;
    IP = P << 2;
    cData[IP++] = len;
    for (i = 0; i < len; i++) {
        cData[IP++] = seq[i];
    }
    while (IP & 3) {
        cData[IP++] = 0;
    }
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" ");
    Serial.print(seq);
}
void STRQ(char seq[])
{
    P = IP >> 2;
    int i;
    int len = strlen(seq);
    data[P++] = STRQP;
    IP = P << 2;
    cData[IP++] = len;
    for (i = 0; i < len; i++) {
        cData[IP++] = seq[i];
    }
    while (IP & 3) {
        cData[IP++] = 0;
    }
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" ");
    Serial.print(seq);
}
void ABORQ(const char seq[])
{
    P = IP >> 2;
    int i;
    int len = strlen(seq);
    data[P++] = ABORQP;
    IP = P << 2;
    cData[IP++] = len;
    for (i = 0; i < len; i++) {
        cData[IP++] = seq[i];
    }
    while (IP & 3) {
        cData[IP++] = 0;
    }
    Serial.println();
    Serial.print(IP, HEX);
    Serial.print(" ");
    Serial.print(seq);
}
