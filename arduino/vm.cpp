#include <Arduino.h>

long rack[256] = { 0 };
long stack[256] = { 0 };

long P;
long IP;
long data[16000] = {};
unsigned char R;
unsigned char S;
long links;
uint8_t* cData;
