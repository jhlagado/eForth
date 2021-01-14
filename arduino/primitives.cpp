#include <Arduino.h>
#include "globals.h"
#include "vm.h"
#include "common.h"
#include "http.h"

/******************************************************************************/
/* PRIMITIVES                                                                 */
/******************************************************************************/

void next(void)
{
    P = data[IP >> 2];
    IP += 4;
    WP = P + 4;
}

void accep()
/* WiFiClient */
{
    while (Serial.available()) {
        len = Serial.readBytes(cData, top);
    }
    Serial.write(cData, len);
    top = len;
}
void qrx(void)
{
    while (Serial.available() == 0) { };
    push Serial.read();
    push - 1;
}

void txsto(void)
{
    Serial.write((unsigned char)top);
    char c = top;
    HTTPout += c;
    pop;
}

void docon(void)
{
    push data[WP >> 2];
}

void dolit(void)
{
    push data[IP >> 2];
    IP += 4;
    next();
}

void dolist(void)
{
    rack[(unsigned char)++R] = IP;
    IP = WP;
    next();
}

void exitt(void)
{
    IP = (long)rack[(unsigned char)R--];
    next();
}

void execu(void)
{
    P = top;
    WP = P + 4;
    pop;
}

void donext(void)
{
    if (rack[(unsigned char)R]) {
        rack[(unsigned char)R] -= 1;
        IP = data[IP >> 2];
    } else {
        IP += 4;
        (unsigned char)R--;
    }
    next();
}

void qbran(void)
{
    if (top == 0)
        IP = data[IP >> 2];
    else
        IP += 4;
    pop;
    next();
}

void bran(void)
{
    IP = data[IP >> 2];
    next();
}

void store(void)
{
    data[top >> 2] = stack[(unsigned char)S--];
    pop;
}

void at(void)
{
    top = data[top >> 2];
}

void cstor(void)
{
    cData[top] = (unsigned char)stack[(unsigned char)S--];
    pop;
}

void cat(void)
{
    top = (long)cData[top];
}

void rpat(void) { }
void rpsto(void) { }

void rfrom(void)
{
    push rack[(unsigned char)R--];
}

void rat(void)
{
    push rack[(unsigned char)R];
}

void tor(void)
{
    rack[(unsigned char)++R] = top;
    pop;
}

void spat(void) { }
void spsto(void) { }

void drop(void)
{
    pop;
}

void dup(void)
{
    stack[(unsigned char)++S] = top;
}

void swap(void)
{
    WP = top;
    top = stack[(unsigned char)S];
    stack[(unsigned char)S] = WP;
}

void over(void)
{
    push stack[(unsigned char)(S - 1)];
}

void zless(void)
{
    top = (top < 0) LOGICAL;
}

void andd(void)
{
    top &= stack[(unsigned char)S--];
}

void orr(void)
{
    top |= stack[(unsigned char)S--];
}

void xorr(void)
{
    top ^= stack[(unsigned char)S--];
}

void uplus(void)
{
    stack[(unsigned char)S] += top;
    top = LOWER(stack[(unsigned char)S], top);
}

void nop(void)
{
    next();
}

void qdup(void)
{
    if (top)
        stack[(unsigned char)++S] = top;
}

void rot(void)
{
    WP = stack[(unsigned char)(S - 1)];
    stack[(unsigned char)(S - 1)] = stack[(unsigned char)S];
    stack[(unsigned char)S] = top;
    top = WP;
}

void ddrop(void)
{
    drop();
    drop();
}

void ddup(void)
{
    over();
    over();
}

void plus(void)
{
    top += stack[(unsigned char)S--];
}

void inver(void)
{
    top = -top - 1;
}

void negat(void)
{
    top = 0 - top;
}

void dnega(void)
{
    inver();
    tor();
    inver();
    push 1;
    uplus();
    rfrom();
    plus();
}

void subb(void)
{
    top = stack[(unsigned char)S--] - top;
}

void abss(void)
{
    if (top < 0)
        top = -top;
}

void great(void)
{
    top = (stack[(unsigned char)S--] > top) LOGICAL;
}

void less(void)
{
    top = (stack[(unsigned char)S--] < top) LOGICAL;
}

void equal(void)
{
    top = (stack[(unsigned char)S--] == top) LOGICAL;
}

void uless(void)
{
    top = LOWER(stack[(unsigned char)S], top) LOGICAL;
    S--;
}

void ummod(void)
{
    d = (long long int)((unsigned long)top);
    m = (long long int)((unsigned long)stack[(unsigned char)S]);
    n = (long long int)((unsigned long)stack[(unsigned char)(S - 1)]);
    n += m << 32;
    pop;
    top = (unsigned long)(n / d);
    stack[(unsigned char)S] = (unsigned long)(n % d);
}
void msmod(void)
{
    d = (signed long long int)((signed long)top);
    m = (signed long long int)((signed long)stack[(unsigned char)S]);
    n = (signed long long int)((signed long)stack[(unsigned char)S - 1]);
    n += m << 32;
    pop;
    top = (signed long)(n / d);
    stack[(unsigned char)S] = (signed long)(n % d);
}
void slmod(void)
{
    if (top != 0) {
        WP = stack[(unsigned char)S] / top;
        stack[(unsigned char)S] %= top;
        top = WP;
    }
}
void mod(void)
{
    top = (top) ? stack[(unsigned char)S--] % top : stack[(unsigned char)S--];
}
void slash(void)
{
    top = (top) ? stack[(unsigned char)S--] / top : (stack[(unsigned char)S--], 0);
}
void umsta(void)
{
    d = (unsigned long long int)top;
    m = (unsigned long long int)stack[(unsigned char)S];
    m *= d;
    top = (unsigned long)(m >> 32);
    stack[(unsigned char)S] = (unsigned long)m;
}
void star(void)
{
    top *= stack[(unsigned char)S--];
}
void mstar(void)
{
    d = (signed long long int)top;
    m = (signed long long int)stack[(unsigned char)S];
    m *= d;
    top = (signed long)(m >> 32);
    stack[(unsigned char)S] = (signed long)m;
}
void ssmod(void)
{
    d = (signed long long int)top;
    m = (signed long long int)stack[(unsigned char)S];
    n = (signed long long int)stack[(unsigned char)(S - 1)];
    n *= m;
    pop;
    top = (signed long)(n / d);
    stack[(unsigned char)S] = (signed long)(n % d);
}
void stasl(void)
{
    d = (signed long long int)top;
    m = (signed long long int)stack[(unsigned char)S];
    n = (signed long long int)stack[(unsigned char)(S - 1)];
    n *= m;
    pop;
    pop;
    top = (signed long)(n / d);
}

void pick(void)
{
    top = stack[(unsigned char)(S - top)];
}

void pstor(void)
{
    data[top >> 2] += stack[(unsigned char)S--], pop;
}

void dstor(void)
{
    data[(top >> 2) + 1] = stack[(unsigned char)S--];
    data[top >> 2] = stack[(unsigned char)S--];
    pop;
}

void dat(void)
{
    push data[top >> 2];
    top = data[(top >> 2) + 1];
}

void count(void)
{
    stack[(unsigned char)++S] = top + 1;
    top = cData[top];
}

void dovar(void)
{
    push WP;
}

void maxx(void)
{
    if (top < stack[(unsigned char)S])
        pop;
    else
        (unsigned char)S--;
}

void minn(void)
{
    if (top < stack[(unsigned char)S])
        (unsigned char)S--;
    else
        pop;
}

void sendPacket(void)
{
}

void poke(void)
{
    Pointer = (long*)top;
    *Pointer = stack[(unsigned char)S--];
    pop;
}

void peeek(void)
{
    Pointer = (long*)top;
    top = *Pointer;
}

void adc(void)
{
    top = (long)analogRead(top);
}

void (*primitives[72])(void) = {
    /* case 0 */ nop,
    /* case 1 */ accep,
    /* case 2 */ qrx,
    /* case 3 */ txsto,
    /* case 4 */ docon,
    /* case 5 */ dolit,
    /* case 6 */ dolist,
    /* case 7 */ exitt,
    /* case 8 */ execu,
    /* case 9 */ donext,
    /* case 10 */ qbran,
    /* case 11 */ bran,
    /* case 12 */ store,
    /* case 13 */ at,
    /* case 14 */ cstor,
    /* case 15 */ cat,
    /* case 16 */ nop,
    /* case 17 */ nop,
    /* case 18 */ rfrom,
    /* case 19 */ rat,
    /* case 20 */ tor,
    /* case 21 */ nop,
    /* case 22 */ nop,
    /* case 23 */ drop,
    /* case 24 */ dup,
    /* case 25 */ swap,
    /* case 26 */ over,
    /* case 27 */ zless,
    /* case 28 */ andd,
    /* case 29 */ orr,
    /* case 30 */ xorr,
    /* case 31 */ uplus,
    /* case 32 */ next,
    /* case 33 */ qdup,
    /* case 34 */ rot,
    /* case 35 */ ddrop,
    /* case 36 */ ddup,
    /* case 37 */ plus,
    /* case 38 */ inver,
    /* case 39 */ negat,
    /* case 40 */ dnega,
    /* case 41 */ subb,
    /* case 42 */ abss,
    /* case 43 */ equal,
    /* case 44 */ uless,
    /* case 45 */ less,
    /* case 46 */ ummod,
    /* case 47 */ msmod,
    /* case 48 */ slmod,
    /* case 49 */ mod,
    /* case 50 */ slash,
    /* case 51 */ umsta,
    /* case 52 */ star,
    /* case 53 */ mstar,
    /* case 54 */ ssmod,
    /* case 55 */ stasl,
    /* case 56 */ pick,
    /* case 57 */ pstor,
    /* case 58 */ dstor,
    /* case 59 */ dat,
    /* case 60 */ count,
    /* case 61 */ dovar,
    /* case 62 */ maxx,
    /* case 63 */ minn,
    /* case 64 */ nop,
    /* case 65 */ sendPacket,
    /* case 66 */ poke,
    /* case 67 */ peeek,
    /* case 68 */ adc,
    /* case 69 */ nop,
    /* case 70 */ nop,
    /* case 71 */ nop
};
