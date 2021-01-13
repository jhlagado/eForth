#include <Arduino.h>
#include "globals.h"
#include "vm.h"
#include "common.h"

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
int as_nop = 0;
int as_accept = 1;
int as_qrx = 2;
int as_txsto = 3;
int as_docon = 4;
int as_dolit = 5;
int as_dolist = 6;
int as_exit = 7;
int as_execu = 8;
int as_donext = 9;
int as_qbran = 10;
int as_bran = 11;
int as_store = 12;
int as_at = 13;
int as_cstor = 14;
int as_cat = 15;
int as_rpat = 16;
int as_rpsto = 17;
int as_rfrom = 18;
int as_rat = 19;
int as_tor = 20;
int as_spat = 21;
int as_spsto = 22;
int as_drop = 23;
int as_dup = 24;
int as_swap = 25;
int as_over = 26;
int as_zless = 27;
int as_andd = 28;
int as_orr = 29;
int as_xorr = 30;
int as_uplus = 31;
int as_next = 32;
int as_qdup = 33;
int as_rot = 34;
int as_ddrop = 35;
int as_ddup = 36;
int as_plus = 37;
int as_inver = 38;
int as_negat = 39;
int as_dnega = 40;
int as_subb = 41;
int as_abss = 42;
int as_equal = 43;
int as_uless = 44;
int as_less = 45;
int as_ummod = 46;
int as_msmod = 47;
int as_slmod = 48;
int as_mod = 49;
int as_slash = 50;
int as_umsta = 51;
int as_star = 52;
int as_mstar = 53;
int as_ssmod = 54;
int as_stasl = 55;
int as_pick = 56;
int as_pstor = 57;
int as_dstor = 58;
int as_dat = 59;
int as_count = 60;
int as_dovar = 61;
int as_max = 62;
int as_min = 63;
int as_tone = 64; //
int as_sendPacket = 65;
int as_poke = 66;
int as_peek = 67;
int as_adc = 68;
int as_pin = 69; //
int as_duty = 70; //
int as_freq = 71; //
