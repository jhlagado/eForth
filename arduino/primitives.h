#pragma once

#include <Arduino.h>

/******************************************************************************/
/* PRIMITIVES                                                                 */
/******************************************************************************/

extern void (*primitives[72])(void);

const int as_nop = 0;
const int as_accept = 1;
const int as_qrx = 2;
const int as_txsto = 3;
const int as_docon = 4;
const int as_dolit = 5;
const int as_dolist = 6;
const int as_exit = 7;
const int as_execu = 8;
const int as_donext = 9;
const int as_qbran = 10;
const int as_bran = 11;
const int as_store = 12;
const int as_at = 13;
const int as_cstor = 14;
const int as_cat = 15;
const int as_rpat = 16;
const int as_rpsto = 17;
const int as_rfrom = 18;
const int as_rat = 19;
const int as_tor = 20;
const int as_spat = 21;
const int as_spsto = 22;
const int as_drop = 23;
const int as_dup = 24;
const int as_swap = 25;
const int as_over = 26;
const int as_zless = 27;
const int as_andd = 28;
const int as_orr = 29;
const int as_xorr = 30;
const int as_uplus = 31;
const int as_next = 32;
const int as_qdup = 33;
const int as_rot = 34;
const int as_ddrop = 35;
const int as_ddup = 36;
const int as_plus = 37;
const int as_inver = 38;
const int as_negat = 39;
const int as_dnega = 40;
const int as_subb = 41;
const int as_abss = 42;
const int as_equal = 43;
const int as_uless = 44;
const int as_less = 45;
const int as_ummod = 46;
const int as_msmod = 47;
const int as_slmod = 48;
const int as_mod = 49;
const int as_slash = 50;
const int as_umsta = 51;
const int as_star = 52;
const int as_mstar = 53;
const int as_ssmod = 54;
const int as_stasl = 55;
const int as_pick = 56;
const int as_pstor = 57;
const int as_dstor = 58;
const int as_dat = 59;
const int as_count = 60;
const int as_dovar = 61;
const int as_max = 62;
const int as_min = 63;
const int as_tone = 64; //
const int as_sendPacket = 65;
const int as_poke = 66;
const int as_peek = 67;
const int as_adc = 68;
const int as_pin = 69; //
const int as_duty = 70; //
const int as_freq = 71; //
