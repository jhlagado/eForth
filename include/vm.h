#pragma once

#define pop top = stack[(unsigned char)S--]
#define push                         \
    stack[(unsigned char)++S] = top; \
    top =
#define popR rack[(unsigned char)R--]
#define pushR rack[(unsigned char)++R]

extern long rack[];
extern long stack[];

extern long P;
extern long IP;
extern long data[];
extern unsigned char R;
extern unsigned char S;
extern long links;
extern uint8_t* cData;
