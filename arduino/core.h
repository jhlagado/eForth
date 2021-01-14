#pragma once

extern int BRAN, QBRAN, DONXT, DOTQP, STRQP, TOR, ABORQP, IMEDD, COMPO;

void HEADER(int lex, const char seq[]);
int CODE(int len, ...);
int COLON(int len, ...);
int LABEL(int len, ...);

void BEGIN(int len, ...);
void AGAIN(int len, ...);
void UNTIL(int len, ...);
void WHILE(int len, ...);
void REPEAT(int len, ...);
void IF(int len, ...);
void ELSE(int len, ...);
void THEN(int len, ...);
void FOR(int len, ...);
void NEXT(int len, ...);
void AFT(int len, ...);
void DOTQ(const char seq[]);
void STRQ(char seq[]);
void ABORQ(const char seq[]);
