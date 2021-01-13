#pragma once

#include <Arduino.h>

/******************************************************************************/
/* PRIMITIVES                                                                 */
/******************************************************************************/

void next(void);
void accep();
void qrx(void);
void txsto(void);
void docon(void);
void dolit(void);
void dolist(void);
void exitt(void);
void execu(void);
void donext(void);
void qbran(void);
void bran(void);
void store(void);
void at(void);
void cstor(void);
void cat(void);
void rpat(void);
void rpsto(void);
void rfrom(void);
void rat(void);
void tor(void);
void spat(void);
void spsto(void);
void drop(void);
void dup(void);
void swap(void);
void over(void);
void zless(void);
void andd(void);
void orr(void);
void xorr(void);
void uplus(void);
void nop(void);
void qdup(void);
void rot(void);
void ddrop(void);
void ddup(void);
void plus(void);
void inver(void);
void negat(void);
void dnega(void);
void subb(void);
void abss(void);
void great(void);
void less(void);
void equal(void);
void uless(void);
void ummod(void);
void msmod(void);
void slmod(void);
void mod(void);
void slash(void);
void umsta(void);
void star(void);
void mstar(void);
void ssmod(void);
void stasl(void);
void pick(void);
void pstor(void);
void dstor(void);
void dat(void);
void count(void);
void dovar(void);
void maxx(void);
void minn(void);
void sendPacket(void);
void poke(void);
void peeek(void);
void adc(void);
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
