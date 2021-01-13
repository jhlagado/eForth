#include <Arduino.h>

#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <TFT_eSPI.h>
#include <WebServer.h>
#include <WiFi.h>

#include "common.h"
#include "config.h"
#include "credentials.h"
#include "index_html.h"
#include "vm.h"
#include "globals.h"
#include "primitives.h"
#include "core.h"

void showNetworkInfo(TFT_eSPI tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setCursor(0, 30);
    tft.setFreeFont(&Orbitron_Light_24);
    tft.println("IP");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println(WiFi.localIP());
    tft.setCursor(0, 90);
    tft.setFreeFont(&FreeSerif12pt7b);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("http://");
    tft.print(deviceName);
    tft.println(".local");
    delay(5000);
}

void printNetworkInfo()
{
    Serial.println();
    Serial.println();
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Navigate to http://");
    Serial.print(deviceName);
    Serial.println(".local");
}

TFT_eSPI tft = TFT_eSPI();
WebServer server(80);

unsigned char bytecode;
// long* Pointer;
// long WP, top, len;
// long long int d, n, m;
// String HTTPin;
// String HTTPout;

int BRAN = 0, QBRAN = 0, DONXT = 0, DOTQP = 0, STRQP = 0, TOR = 0, ABORQP = 0;

int IMEDD = 0x80;
int COMPO = 0x40;

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

void CheckSum()
{
    int i;
    char sum = 0;
    Serial.println();
    Serial.printf("%4x ", IP);
    for (i = 0; i < 32; i++) {
        sum += cData[IP];
        Serial.printf("%2x", cData[IP++]);
    }
    Serial.printf(" %2x", sum);
}


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

static void returnFail(String msg)
{
    server.send(500, "text/plain", msg + "\r\n");
}

static void handleInput()
{
    if (!server.hasArg("cmd")) {
        return returnFail("Missing Input");
    }
    HTTPin = server.arg("cmd");
    HTTPout = "";
    Serial.println(HTTPin); // line cleaned up
    len = HTTPin.length();
    HTTPin.getBytes(cData, len);
    //Serial.println("Enter Forth.");
    data[0x66] = 0; // >IN
    data[0x67] = len; // #TIB
    data[0x68] = 0; // 'TIB
    P = 0x180; // EVAL
    WP = 0x184;
    evaluate();
    Serial.print(HTTPout.length());
    server.setContentLength(HTTPout.length());
    server.send(200, "text/plain", HTTPout);
}

void setup()
{
    P = 0x180;
    WP = 0x184;
    IP = 0;
    S = 0;
    R = 0;
    top = 0;
    cData = (uint8_t*)data;
    Serial.begin(monitorSpeed);
    delay(100);

    // attempt to connect to Wifi network:
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    if (!MDNS.begin(deviceName)) {
        Serial.print("Error starting MDNS");
    }

    server.begin();

    IP = 512;
    R = 0;
    HEADER(3, "HLD");
    int HLD = CODE(8, as_docon, as_next, 0, 0, 0X90, 1, 0, 0);
    HEADER(4, "SPAN");
    int SPAN = CODE(8, as_docon, as_next, 0, 0, 0X94, 1, 0, 0);
    HEADER(3, ">IN");
    int INN = CODE(8, as_docon, as_next, 0, 0, 0X98, 1, 0, 0);
    HEADER(4, "#TIB");
    int NTIB = CODE(8, as_docon, as_next, 0, 0, 0X9C, 1, 0, 0);
    HEADER(4, "'TIB");
    int TTIB = CODE(8, as_docon, as_next, 0, 0, 0XA0, 1, 0, 0);
    HEADER(4, "BASE");
    int BASE = CODE(8, as_docon, as_next, 0, 0, 0XA4, 1, 0, 0);
    HEADER(7, "CONTEXT");
    int CNTXT = CODE(8, as_docon, as_next, 0, 0, 0XA8, 1, 0, 0);
    HEADER(2, "CP");
    int CP = CODE(8, as_docon, as_next, 0, 0, 0XAC, 1, 0, 0);
    HEADER(4, "LAST");
    int LAST = CODE(8, as_docon, as_next, 0, 0, 0XB0, 1, 0, 0);
    HEADER(5, "'EVAL");
    int TEVAL = CODE(8, as_docon, as_next, 0, 0, 0XB4, 1, 0, 0);
    HEADER(6, "'ABORT");
    int TABRT = CODE(8, as_docon, as_next, 0, 0, 0XB8, 1, 0, 0);
    HEADER(3, "tmp");
    int TEMP = CODE(8, as_docon, as_next, 0, 0, 0XBC, 1, 0, 0);
    HEADER(1, "Z");
    int Z = CODE(8, as_docon, as_next, 0, 0, 0, 0, 0, 0);
    HEADER(4, "ppqn");
    int PPQN = CODE(8, as_docon, as_next, 0, 0, 0XC0, 1, 0, 0);
    HEADER(7, "channel");
    int CHANN = CODE(8, as_docon, as_next, 0, 0, 0XC4, 1, 0, 0);

    HEADER(3, "NOP");
    int NOP = CODE(4, as_nop, as_next, 0, 0);
    HEADER(6, "ACCEPT");
    int ACCEP = CODE(4, as_accept, as_next, 0, 0);
    HEADER(4, "?KEY");
    int QKEY = CODE(4, as_qrx, as_next, 0, 0);
    HEADER(4, "EMIT");
    int EMIT = CODE(4, as_txsto, as_next, 0, 0);
    HEADER(5, "DOLIT");
    int DOLIT = CODE(4, as_dolit, as_next, 0, 0);
    HEADER(6, "DOLIST");
    int DOLST = CODE(4, as_dolist, as_next, 0, 0);
    HEADER(4, "EXIT");
    int EXITT = CODE(4, as_exit, as_next, 0, 0);
    HEADER(7, "EXECUTE");
    int EXECU = CODE(4, as_execu, as_next, 0, 0);
    HEADER(6, "DONEXT");
    DONXT = CODE(4, as_donext, as_next, 0, 0);
    HEADER(7, "QBRANCH");
    QBRAN = CODE(4, as_qbran, as_next, 0, 0);
    HEADER(6, "BRANCH");
    BRAN = CODE(4, as_bran, as_next, 0, 0);
    HEADER(1, "!");
    int STORE = CODE(4, as_store, as_next, 0, 0);
    HEADER(1, "@");
    int AT = CODE(4, as_at, as_next, 0, 0);
    HEADER(2, "C!");
    int CSTOR = CODE(4, as_cstor, as_next, 0, 0);
    HEADER(2, "C@");
    int CAT = CODE(4, as_cat, as_next, 0, 0);
    HEADER(2, "R>");
    int RFROM = CODE(4, as_rfrom, as_next, 0, 0);
    HEADER(2, "R@");
    int RAT = CODE(4, as_rat, as_next, 0, 0);
    HEADER(2, ">R");
    TOR = CODE(4, as_tor, as_next, 0, 0);
    HEADER(4, "DROP");
    int DROP = CODE(4, as_drop, as_next, 0, 0);
    HEADER(3, "DUP");
    int DUPP = CODE(4, as_dup, as_next, 0, 0);
    HEADER(4, "SWAP");
    int SWAP = CODE(4, as_swap, as_next, 0, 0);
    HEADER(4, "OVER");
    int OVER = CODE(4, as_over, as_next, 0, 0);
    HEADER(2, "0<");
    int ZLESS = CODE(4, as_zless, as_next, 0, 0);
    HEADER(3, "AND");
    int ANDD = CODE(4, as_andd, as_next, 0, 0);
    HEADER(2, "OR");
    int ORR = CODE(4, as_orr, as_next, 0, 0);
    HEADER(3, "XOR");
    int XORR = CODE(4, as_xorr, as_next, 0, 0);
    HEADER(3, "UM+");
    int UPLUS = CODE(4, as_uplus, as_next, 0, 0);
    HEADER(4, "?DUP");
    int QDUP = CODE(4, as_qdup, as_next, 0, 0);
    HEADER(3, "ROT");
    int ROT = CODE(4, as_rot, as_next, 0, 0);
    HEADER(5, "2DROP");
    int DDROP = CODE(4, as_ddrop, as_next, 0, 0);
    HEADER(4, "2DUP");
    int DDUP = CODE(4, as_ddup, as_next, 0, 0);
    HEADER(1, "+");
    int PLUS = CODE(4, as_plus, as_next, 0, 0);
    HEADER(3, "NOT");
    int INVER = CODE(4, as_inver, as_next, 0, 0);
    HEADER(6, "NEGATE");
    int NEGAT = CODE(4, as_negat, as_next, 0, 0);
    HEADER(7, "DNEGATE");
    int DNEGA = CODE(4, as_dnega, as_next, 0, 0);
    HEADER(1, "-");
    int SUBBB = CODE(4, as_subb, as_next, 0, 0);
    HEADER(3, "ABS");
    int ABSS = CODE(4, as_abss, as_next, 0, 0);
    HEADER(1, "=");
    int EQUAL = CODE(4, as_equal, as_next, 0, 0);
    HEADER(2, "U<");
    int ULESS = CODE(4, as_uless, as_next, 0, 0);
    HEADER(1, "<");
    int LESS = CODE(4, as_less, as_next, 0, 0);
    HEADER(6, "UM/MOD");
    int UMMOD = CODE(4, as_ummod, as_next, 0, 0);
    HEADER(5, "M/MOD");
    int MSMOD = CODE(4, as_msmod, as_next, 0, 0);
    HEADER(4, "/MOD");
    int SLMOD = CODE(4, as_slmod, as_next, 0, 0);
    HEADER(3, "MOD");
    int MODD = CODE(4, as_mod, as_next, 0, 0);
    HEADER(1, "/");
    int SLASH = CODE(4, as_slash, as_next, 0, 0);
    HEADER(3, "UM*");
    int UMSTA = CODE(4, as_umsta, as_next, 0, 0);
    HEADER(1, "*");
    int STAR = CODE(4, as_star, as_next, 0, 0);
    HEADER(2, "M*");
    int MSTAR = CODE(4, as_mstar, as_next, 0, 0);
    HEADER(5, "*/MOD");
    int SSMOD = CODE(4, as_ssmod, as_next, 0, 0);
    HEADER(2, "*/");
    int STASL = CODE(4, as_stasl, as_next, 0, 0);
    HEADER(4, "PICK");
    int PICK = CODE(4, as_pick, as_next, 0, 0);
    HEADER(2, "+!");
    int PSTOR = CODE(4, as_pstor, as_next, 0, 0);
    HEADER(2, "2!");
    int DSTOR = CODE(4, as_dstor, as_next, 0, 0);
    HEADER(2, "2@");
    int DAT = CODE(4, as_dat, as_next, 0, 0);
    HEADER(5, "COUNT");
    int COUNT = CODE(4, as_count, as_next, 0, 0);
    HEADER(3, "MAX");
    int MAX = CODE(4, as_max, as_next, 0, 0);
    HEADER(3, "MIN");
    int MIN = CODE(4, as_min, as_next, 0, 0);
    HEADER(2, "BL");
    int BLANK = CODE(8, as_docon, as_next, 0, 0, 32, 0, 0, 0);
    HEADER(4, "CELL");
    int CELL = CODE(8, as_docon, as_next, 0, 0, 4, 0, 0, 0);
    HEADER(5, "CELL+");
    int CELLP = CODE(8, as_docon, as_plus, as_next, 0, 4, 0, 0, 0);
    HEADER(5, "CELL-");
    int CELLM = CODE(8, as_docon, as_subb, as_next, 0, 4, 0, 0, 0);
    HEADER(5, "CELLS");
    int CELLS = CODE(8, as_docon, as_star, as_next, 0, 4, 0, 0, 0);
    HEADER(5, "CELL/");
    int CELLD = CODE(8, as_docon, as_slash, as_next, 0, 4, 0, 0, 0);
    HEADER(2, "1+");
    int ONEP = CODE(8, as_docon, as_plus, as_next, 0, 1, 0, 0, 0);
    HEADER(2, "1-");
    int ONEM = CODE(8, as_docon, as_subb, as_next, 0, 1, 0, 0, 0);
    HEADER(2, "2+");
    int TWOP = CODE(8, as_docon, as_plus, as_next, 0, 2, 0, 0, 0);
    HEADER(2, "2-");
    int TWOM = CODE(8, as_docon, as_subb, as_next, 0, 2, 0, 0, 0);
    HEADER(2, "2*");
    int TWOST = CODE(8, as_docon, as_star, as_next, 0, 2, 0, 0, 0);
    HEADER(2, "2/");
    int TWOS = CODE(8, as_docon, as_slash, as_next, 0, 2, 0, 0, 0);
    HEADER(10, "sendPacket");
    int SENDP = CODE(4, as_sendPacket, as_next, 0, 0);
    HEADER(4, "POKE");
    int POKE = CODE(4, as_poke, as_next, 0, 0);
    HEADER(4, "PEEK");
    int PEEK = CODE(4, as_peek, as_next, 0, 0);
    HEADER(3, "ADC");
    int ADC = CODE(4, as_adc, as_next, 0, 0);
    HEADER(3, "PIN");
    int PIN = CODE(4, as_pin, as_next, 0, 0);
    HEADER(4, "TONE");
    int TONE = CODE(4, as_tone, as_next, 0, 0);
    HEADER(4, "DUTY");
    int DUTY = CODE(4, as_duty, as_next, 0, 0);
    HEADER(4, "FREQ");
    int FREQ = CODE(4, as_freq, as_next, 0, 0);

    HEADER(3, "KEY");
    int KEY = COLON(0);
    BEGIN(1, QKEY);
    UNTIL(1, EXITT);
    HEADER(6, "WITHIN");
    int WITHI = COLON(7, OVER, SUBBB, TOR, SUBBB, RFROM, ULESS, EXITT);
    HEADER(5, ">CHAR");
    int TCHAR = COLON(8, DOLIT, 0x7F, ANDD, DUPP, DOLIT, 127, BLANK, WITHI);
    IF(3, DROP, DOLIT, 0X5F);
    THEN(1, EXITT);
    HEADER(7, "ALIGNED");
    int ALIGN = COLON(7, DOLIT, 3, PLUS, DOLIT, 0XFFFFFFFC, ANDD, EXITT);
    HEADER(4, "HERE");
    int HERE = COLON(3, CP, AT, EXITT);
    HEADER(3, "PAD");
    int PAD = COLON(5, HERE, DOLIT, 80, PLUS, EXITT);
    HEADER(3, "TIB");
    int TIB = COLON(3, TTIB, AT, EXITT);
    HEADER(8, "@EXECUTE");
    int ATEXE = COLON(2, AT, QDUP);
    IF(1, EXECU);
    THEN(1, EXITT);
    HEADER(5, "CMOVE");
    int CMOVEE = COLON(0);
    FOR(0);
    AFT(8, OVER, CAT, OVER, CSTOR, TOR, ONEP, RFROM, ONEP);
    THEN(0);
    NEXT(2, DDROP, EXITT);
    HEADER(4, "MOVE");
    int MOVE = COLON(1, CELLD);
    FOR(0);
    AFT(8, OVER, AT, OVER, STORE, TOR, CELLP, RFROM, CELLP);
    THEN(0);
    NEXT(2, DDROP, EXITT);
    HEADER(4, "FILL");
    int FILL = COLON(1, SWAP);
    FOR(1, SWAP);
    AFT(3, DDUP, CSTOR, ONEP);
    THEN(0);
    NEXT(2, DDROP, EXITT);
    HEADER(5, "DIGIT");
    int DIGIT = COLON(12, DOLIT, 9, OVER, LESS, DOLIT, 7, ANDD, PLUS, DOLIT, 0X30, PLUS, EXITT);
    HEADER(7, "EXTRACT");
    int EXTRC = COLON(7, DOLIT, 0, SWAP, UMMOD, SWAP, DIGIT, EXITT);
    HEADER(2, "<#");
    int BDIGS = COLON(4, PAD, HLD, STORE, EXITT);
    HEADER(4, "HOLD");
    int HOLD = COLON(8, HLD, AT, ONEM, DUPP, HLD, STORE, CSTOR, EXITT);
    HEADER(1, "#");
    int DIG = COLON(5, BASE, AT, EXTRC, HOLD, EXITT);
    HEADER(2, "#S");
    int DIGS = COLON(0);
    BEGIN(2, DIG, DUPP);
    WHILE(0);
    REPEAT(1, EXITT);
    HEADER(4, "SIGN");
    int SIGN = COLON(1, ZLESS);
    IF(3, DOLIT, 0X2D, HOLD);
    THEN(1, EXITT);
    HEADER(2, "#>");
    int EDIGS = COLON(7, DROP, HLD, AT, PAD, OVER, SUBBB, EXITT);
    HEADER(3, "str");
    int STRR = COLON(9, DUPP, TOR, ABSS, BDIGS, DIGS, RFROM, SIGN, EDIGS, EXITT);
    HEADER(3, "HEX");
    int HEXX = COLON(5, DOLIT, 16, BASE, STORE, EXITT);
    HEADER(7, "DECIMAL");
    int DECIM = COLON(5, DOLIT, 10, BASE, STORE, EXITT);
    HEADER(6, "wupper");
    int UPPER = COLON(4, DOLIT, 0x5F5F5F5F, ANDD, EXITT);
    HEADER(6, ">upper");
    int TOUPP = COLON(6, DUPP, DOLIT, 0x61, DOLIT, 0x7B, WITHI);
    IF(3, DOLIT, 0x5F, ANDD);
    THEN(1, EXITT);
    HEADER(6, "DIGIT?");
    int DIGTQ = COLON(9, TOR, TOUPP, DOLIT, 0X30, SUBBB, DOLIT, 9, OVER, LESS);
    IF(8, DOLIT, 7, SUBBB, DUPP, DOLIT, 10, LESS, ORR);
    THEN(4, DUPP, RFROM, ULESS, EXITT);
    HEADER(7, "NUMBER?");
    int NUMBQ = COLON(12, BASE, AT, TOR, DOLIT, 0, OVER, COUNT, OVER, CAT, DOLIT, 0X24, EQUAL);
    IF(5, HEXX, SWAP, ONEP, SWAP, ONEM);
    THEN(13, OVER, CAT, DOLIT, 0X2D, EQUAL, TOR, SWAP, RAT, SUBBB, SWAP, RAT, PLUS, QDUP);
    IF(1, ONEM);
    FOR(6, DUPP, TOR, CAT, BASE, AT, DIGTQ);
    WHILE(7, SWAP, BASE, AT, STAR, PLUS, RFROM, ONEP);
    NEXT(2, DROP, RAT);
    IF(1, NEGAT);
    THEN(1, SWAP);
    ELSE(6, RFROM, RFROM, DDROP, DDROP, DOLIT, 0);
    THEN(1, DUPP);
    THEN(6, RFROM, DDROP, RFROM, BASE, STORE, EXITT);
    HEADER(5, "SPACE");
    int SPACE = COLON(3, BLANK, EMIT, EXITT);
    HEADER(5, "CHARS");
    int CHARS = COLON(4, SWAP, DOLIT, 0, MAX);
    FOR(0);
    AFT(2, DUPP, EMIT);
    THEN(0);
    NEXT(2, DROP, EXITT);
    HEADER(6, "SPACES");
    int SPACS = COLON(3, BLANK, CHARS, EXITT);
    HEADER(4, "TYPE");
    int TYPES = COLON(0);
    FOR(0);
    AFT(5, DUPP, CAT, TCHAR, EMIT, ONEP);
    THEN(0);
    NEXT(2, DROP, EXITT);
    HEADER(2, "CR");
    int CR = COLON(7, DOLIT, 10, DOLIT, 13, EMIT, EMIT, EXITT);
    HEADER(3, "do$");
    int DOSTR = COLON(10, RFROM, RAT, RFROM, COUNT, PLUS, ALIGN, TOR, SWAP, TOR, EXITT);
    HEADER(3, "$\"|");
    int STRQP = COLON(2, DOSTR, EXITT);
    HEADER(3, ".\"|");
    DOTQP = COLON(4, DOSTR, COUNT, TYPES, EXITT);
    HEADER(2, ".R");
    int DOTR = COLON(8, TOR, STRR, RFROM, OVER, SUBBB, SPACS, TYPES, EXITT);
    HEADER(3, "U.R");
    int UDOTR = COLON(10, TOR, BDIGS, DIGS, EDIGS, RFROM, OVER, SUBBB, SPACS, TYPES, EXITT);
    HEADER(2, "U.");
    int UDOT = COLON(6, BDIGS, DIGS, EDIGS, SPACE, TYPES, EXITT);
    HEADER(1, ".");
    int DOT = COLON(5, BASE, AT, DOLIT, 10, XORR);
    IF(3, UDOT, EXITT);
    THEN(4, STRR, SPACE, TYPES, EXITT);
    HEADER(1, "?");
    int QUEST = COLON(3, AT, DOT, EXITT);
    HEADER(7, "(parse)");
    int PARS = COLON(5, TEMP, CSTOR, OVER, TOR, DUPP);
    IF(5, ONEM, TEMP, CAT, BLANK, EQUAL);
    IF(0);
    FOR(6, BLANK, OVER, CAT, SUBBB, ZLESS, INVER);
    WHILE(1, ONEP);
    NEXT(6, RFROM, DROP, DOLIT, 0, DUPP, EXITT);
    THEN(1, RFROM);
    THEN(2, OVER, SWAP);
    FOR(9, TEMP, CAT, OVER, CAT, SUBBB, TEMP, CAT, BLANK, EQUAL);
    IF(1, ZLESS);
    THEN(0);
    WHILE(1, ONEP);
    NEXT(2, DUPP, TOR);
    ELSE(5, RFROM, DROP, DUPP, ONEP, TOR);
    THEN(6, OVER, SUBBB, RFROM, RFROM, SUBBB, EXITT);
    THEN(4, OVER, RFROM, SUBBB, EXITT);
    HEADER(5, "PACK$");
    int PACKS = COLON(18, DUPP, TOR, DDUP, PLUS, DOLIT, 0xFFFFFFFC, ANDD, DOLIT, 0, SWAP, STORE, DDUP, CSTOR, ONEP, SWAP, CMOVEE, RFROM, EXITT);
    HEADER(5, "PARSE");
    int PARSE = COLON(15, TOR, TIB, INN, AT, PLUS, NTIB, AT, INN, AT, SUBBB, RFROM, PARS, INN, PSTOR, EXITT);
    HEADER(5, "TOKEN");
    int TOKEN = COLON(9, BLANK, PARSE, DOLIT, 0x1F, MIN, HERE, CELLP, PACKS, EXITT);
    HEADER(4, "WORD");
    int WORDD = COLON(5, PARSE, HERE, CELLP, PACKS, EXITT);
    HEADER(5, "NAME>");
    int NAMET = COLON(7, COUNT, DOLIT, 0x1F, ANDD, PLUS, ALIGN, EXITT);
    HEADER(5, "SAME?");
    int SAMEQ = COLON(4, DOLIT, 0x1F, ANDD, CELLD);
    FOR(0);
    AFT(18, OVER, RAT, DOLIT, 4, STAR, PLUS, AT, UPPER, OVER, RAT, DOLIT, 4, STAR, PLUS, AT, UPPER, SUBBB, QDUP);
    IF(3, RFROM, DROP, EXITT);
    THEN(0);
    THEN(0);
    NEXT(3, DOLIT, 0, EXITT);
    HEADER(4, "find");
    int FIND = COLON(10, SWAP, DUPP, AT, TEMP, STORE, DUPP, AT, TOR, CELLP, SWAP);
    BEGIN(2, AT, DUPP);
    IF(9, DUPP, AT, DOLIT, 0xFFFFFF3F, ANDD, UPPER, RAT, UPPER, XORR);
    IF(3, CELLP, DOLIT, 0XFFFFFFFF);
    ELSE(4, CELLP, TEMP, AT, SAMEQ);
    THEN(0);
    ELSE(6, RFROM, DROP, SWAP, CELLM, SWAP, EXITT);
    THEN(0);
    WHILE(2, CELLM, CELLM);
    REPEAT(9, RFROM, DROP, SWAP, DROP, CELLM, DUPP, NAMET, SWAP, EXITT);
    HEADER(5, "NAME?");
    int NAMEQ = COLON(3, CNTXT, FIND, EXITT);
    HEADER(6, "EXPECT");
    int EXPEC = COLON(5, ACCEP, SPAN, STORE, DROP, EXITT);
    HEADER(5, "QUERY");
    int QUERY = COLON(12, TIB, DOLIT, 0X100, ACCEP, NTIB, STORE, DROP, DOLIT, 0, INN, STORE, EXITT);
    HEADER(5, "ABORT");
    int ABORT = COLON(4, NOP, TABRT, ATEXE, EXITT);
    HEADER(6, "abort\"");
    ABORQP = COLON(0);
    IF(4, DOSTR, COUNT, TYPES, ABORT);
    THEN(3, DOSTR, DROP, EXITT);
    HEADER(5, "ERROR");
    int ERRORR = COLON(8, SPACE, COUNT, TYPES, DOLIT, 0x3F, EMIT, CR, ABORT);
    HEADER(10, "$INTERPRET");
    int INTER = COLON(2, NAMEQ, QDUP);
    IF(4, CAT, DOLIT, COMPO, ANDD);
    ABORQ(" compile only");
    int INTER0 = LABEL(2, EXECU, EXITT);
    THEN(1, NUMBQ);
    IF(1, EXITT);
    THEN(1, ERRORR);
    HEADER(IMEDD + 1, "[");
    int LBRAC = COLON(5, DOLIT, INTER, TEVAL, STORE, EXITT);
    HEADER(3, ".OK");
    int DOTOK = COLON(6, CR, DOLIT, INTER, TEVAL, AT, EQUAL);
    IF(14, TOR, TOR, TOR, DUPP, DOT, RFROM, DUPP, DOT, RFROM, DUPP, DOT, RFROM, DUPP, DOT);
    DOTQ(" ok>");
    THEN(1, EXITT);
    HEADER(4, "EVAL");
    int EVAL = COLON(1, LBRAC);
    BEGIN(3, TOKEN, DUPP, AT);
    WHILE(2, TEVAL, ATEXE);
    REPEAT(4, DROP, DOTOK, NOP, EXITT);
    HEADER(4, "QUIT");
    int QUITT = COLON(1, LBRAC);
    BEGIN(2, QUERY, EVAL);
    AGAIN(0);
    HEADER(4, "LOAD");
    int LOAD = COLON(10, NTIB, STORE, TTIB, STORE, DOLIT, 0, INN, STORE, EVAL, EXITT);
    HEADER(1, ",");
    int COMMA = COLON(7, HERE, DUPP, CELLP, CP, STORE, STORE, EXITT);
    HEADER(IMEDD + 7, "LITERAL");
    int LITER = COLON(5, DOLIT, DOLIT, COMMA, COMMA, EXITT);
    HEADER(5, "ALLOT");
    int ALLOT = COLON(4, ALIGN, CP, PSTOR, EXITT);
    HEADER(3, "$,\"");
    int STRCQ = COLON(9, DOLIT, 0X22, WORDD, COUNT, PLUS, ALIGN, CP, STORE, EXITT);
    HEADER(7, "?UNIQUE");
    int UNIQU = COLON(3, DUPP, NAMEQ, QDUP);
    IF(6, COUNT, DOLIT, 0x1F, ANDD, SPACE, TYPES);
    DOTQ(" reDef");
    THEN(2, DROP, EXITT);
    HEADER(3, "$,n");
    int SNAME = COLON(2, DUPP, AT);
    IF(14, UNIQU, DUPP, NAMET, CP, STORE, DUPP, LAST, STORE, CELLM, CNTXT, AT, SWAP, STORE, EXITT);
    THEN(1, ERRORR);
    HEADER(1, "'");
    int TICK = COLON(2, TOKEN, NAMEQ);
    IF(1, EXITT);
    THEN(1, ERRORR);
    HEADER(IMEDD + 9, "[COMPILE]");
    int BCOMP = COLON(3, TICK, COMMA, EXITT);
    HEADER(7, "COMPILE");
    int COMPI = COLON(7, RFROM, DUPP, AT, COMMA, CELLP, TOR, EXITT);
    HEADER(8, "$COMPILE");
    int SCOMP = COLON(2, NAMEQ, QDUP);
    IF(4, AT, DOLIT, IMEDD, ANDD);
    IF(1, EXECU);
    ELSE(1, COMMA);
    THEN(1, EXITT);
    THEN(1, NUMBQ);
    IF(2, LITER, EXITT);
    THEN(1, ERRORR);
    HEADER(5, "OVERT");
    int OVERT = COLON(5, LAST, AT, CNTXT, STORE, EXITT);
    HEADER(1, "]");
    int RBRAC = COLON(5, DOLIT, SCOMP, TEVAL, STORE, EXITT);
    HEADER(1, ":");
    int COLN = COLON(7, TOKEN, SNAME, RBRAC, DOLIT, 0x6, COMMA, EXITT);
    HEADER(IMEDD + 1, ";");
    int SEMIS = COLON(6, DOLIT, EXITT, COMMA, LBRAC, OVERT, EXITT);
    HEADER(3, "dm+");
    int DMP = COLON(4, OVER, DOLIT, 6, UDOTR);
    FOR(0);
    AFT(6, DUPP, AT, DOLIT, 9, UDOTR, CELLP);
    THEN(0);
    NEXT(1, EXITT);
    HEADER(4, "DUMP");
    int DUMP = COLON(10, BASE, AT, TOR, HEXX, DOLIT, 0x1F, PLUS, DOLIT, 0x20, SLASH);
    FOR(0);
    AFT(10, CR, DOLIT, 8, DDUP, DMP, TOR, SPACE, CELLS, TYPES, RFROM);
    THEN(0);
    NEXT(5, DROP, RFROM, BASE, STORE, EXITT);
    HEADER(5, ">NAME");
    int TNAME = COLON(1, CNTXT);
    BEGIN(2, AT, DUPP);
    WHILE(3, DDUP, NAMET, XORR);
    IF(1, ONEM);
    ELSE(3, SWAP, DROP, EXITT);
    THEN(0);
    REPEAT(3, SWAP, DROP, EXITT);
    HEADER(3, ".ID");
    int DOTID = COLON(7, COUNT, DOLIT, 0x1F, ANDD, TYPES, SPACE, EXITT);
    HEADER(5, "WORDS");
    int WORDS = COLON(6, CR, CNTXT, DOLIT, 0, TEMP, STORE);
    BEGIN(2, AT, QDUP);
    WHILE(9, DUPP, SPACE, DOTID, CELLM, TEMP, AT, DOLIT, 0x10, LESS);
    IF(4, DOLIT, 1, TEMP, PSTOR);
    ELSE(5, CR, DOLIT, 0, TEMP, STORE);
    THEN(0);
    REPEAT(1, EXITT);
    HEADER(6, "FORGET");
    int FORGT = COLON(3, TOKEN, NAMEQ, QDUP);
    IF(12, CELLM, DUPP, CP, STORE, AT, DUPP, CNTXT, STORE, LAST, STORE, DROP, EXITT);
    THEN(1, ERRORR);
    HEADER(4, "COLD");
    int COLD = COLON(1, CR);
    DOTQ("esp32forth V6.3, 2019 ");
    int DOTQ1 = LABEL(2, CR, EXITT);
    HEADER(4, "LINE");
    int LINE = COLON(2, DOLIT, 0x7);
    FOR(6, DUPP, PEEK, DOLIT, 0x9, UDOTR, CELLP);
    NEXT(1, EXITT);
    HEADER(2, "PP");
    int PP = COLON(0);
    FOR(0);
    AFT(7, CR, DUPP, DOLIT, 0x9, UDOTR, SPACE, LINE);
    THEN(0);
    NEXT(1, EXITT);
    HEADER(2, "P0");
    int P0 = COLON(4, DOLIT, 0x3FF44004, POKE, EXITT);
    HEADER(3, "P0S");
    int P0S = COLON(4, DOLIT, 0x3FF44008, POKE, EXITT);
    HEADER(3, "P0C");
    int P0C = COLON(4, DOLIT, 0x3FF4400C, POKE, EXITT);
    HEADER(2, "P1");
    int P1 = COLON(4, DOLIT, 0x3FF44010, POKE, EXITT);
    HEADER(3, "P1S");
    int P1S = COLON(4, DOLIT, 0x3FF44014, POKE, EXITT);
    HEADER(3, "P1C");
    int P1C = COLON(4, DOLIT, 0x3FF44018, POKE, EXITT);
    HEADER(4, "P0EN");
    int P0EN = COLON(4, DOLIT, 0x3FF44020, POKE, EXITT);
    HEADER(5, "P0ENS");
    int P0ENS = COLON(4, DOLIT, 0x3FF44024, POKE, EXITT);
    HEADER(5, "P0ENC");
    int P0ENC = COLON(4, DOLIT, 0x3FF44028, POKE, EXITT);
    HEADER(4, "P1EN");
    int P1EN = COLON(4, DOLIT, 0x3FF4402C, POKE, EXITT);
    HEADER(5, "P1ENS");
    int P1ENS = COLON(4, DOLIT, 0x3FF44030, POKE, EXITT);
    HEADER(5, "P1ENC");
    int P1ENC = COLON(4, DOLIT, 0x3FF44034, POKE, EXITT);
    HEADER(4, "P0IN");
    int P0IN = COLON(5, DOLIT, 0x3FF4403C, PEEK, DOT, EXITT);
    HEADER(4, "P1IN");
    int P1IN = COLON(5, DOLIT, 0x3FF44040, PEEK, DOT, EXITT);
    HEADER(3, "PPP");
    int PPP = COLON(7, DOLIT, 0x3FF44000, DOLIT, 3, PP, DROP, EXITT);
    HEADER(5, "EMITT");
    int EMITT = COLON(2, DOLIT, 0x3);
    FOR(8, DOLIT, 0, DOLIT, 0x100, MSMOD, SWAP, TCHAR, EMIT);
    NEXT(2, DROP, EXITT);
    HEADER(5, "TYPEE");
    int TYPEE = COLON(3, SPACE, DOLIT, 0x7);
    FOR(4, DUPP, PEEK, EMITT, CELLP);
    NEXT(2, DROP, EXITT);
    HEADER(4, "PPPP");
    int PPPP = COLON(0);
    FOR(0);
    AFT(10, CR, DUPP, DUPP, DOLIT, 0x9, UDOTR, SPACE, LINE, SWAP, TYPEE);
    THEN(0);
    NEXT(1, EXITT);
    HEADER(3, "KKK");
    int KKK = COLON(7, DOLIT, 0x3FF59000, DOLIT, 0x10, PP, DROP, EXITT);
    HEADER(IMEDD + 4, "THEN");
    int THENN = COLON(4, HERE, SWAP, STORE, EXITT);
    HEADER(IMEDD + 3, "FOR");
    int FORR = COLON(4, COMPI, TOR, HERE, EXITT);
    HEADER(IMEDD + 5, "BEGIN");
    int BEGIN = COLON(2, HERE, EXITT);
    HEADER(IMEDD + 4, "NEXT");
    int NEXT = COLON(4, COMPI, DONXT, COMMA, EXITT);
    HEADER(IMEDD + 5, "UNTIL");
    int UNTIL = COLON(4, COMPI, QBRAN, COMMA, EXITT);
    HEADER(IMEDD + 5, "AGAIN");
    int AGAIN = COLON(4, COMPI, BRAN, COMMA, EXITT);
    HEADER(IMEDD + 2, "IF");
    int IFF = COLON(7, COMPI, QBRAN, HERE, DOLIT, 0, COMMA, EXITT);
    HEADER(IMEDD + 5, "AHEAD");
    int AHEAD = COLON(7, COMPI, BRAN, HERE, DOLIT, 0, COMMA, EXITT);
    HEADER(IMEDD + 6, "REPEAT");
    int REPEA = COLON(3, AGAIN, THENN, EXITT);
    HEADER(IMEDD + 3, "AFT");
    int AFT = COLON(5, DROP, AHEAD, HERE, SWAP, EXITT);
    HEADER(IMEDD + 4, "ELSE");
    int ELSEE = COLON(4, AHEAD, SWAP, THENN, EXITT);
    HEADER(IMEDD + 5, "WHILE");
    int WHILEE = COLON(3, IFF, SWAP, EXITT);
    HEADER(IMEDD + 6, "ABORT\"");
    int ABRTQ = COLON(6, DOLIT, ABORQP, HERE, STORE, STRCQ, EXITT);
    HEADER(IMEDD + 2, "$\"");
    int STRQ = COLON(6, DOLIT, STRQP, HERE, STORE, STRCQ, EXITT);
    HEADER(IMEDD + 2, ".\"");
    int DOTQQ = COLON(6, DOLIT, DOTQP, HERE, STORE, STRCQ, EXITT);
    HEADER(4, "CODE");
    int CODE = COLON(5, TOKEN, SNAME, OVERT, ALIGN, EXITT);
    HEADER(6, "CREATE");
    int CREAT = COLON(5, CODE, DOLIT, 0x203D, COMMA, EXITT);
    HEADER(8, "VARIABLE");
    int VARIA = COLON(5, CREAT, DOLIT, 0, COMMA, EXITT);
    HEADER(8, "CONSTANT");
    int CONST = COLON(6, CODE, DOLIT, 0x2004, COMMA, COMMA, EXITT);
    HEADER(IMEDD + 2, ".(");
    int DOTPR = COLON(5, DOLIT, 0X29, PARSE, TYPES, EXITT);
    HEADER(IMEDD + 1, "\\");
    int BKSLA = COLON(5, DOLIT, 0xA, WORDD, DROP, EXITT);
    HEADER(IMEDD + 1, "(");
    int PAREN = COLON(5, DOLIT, 0X29, PARSE, DDROP, EXITT);
    HEADER(12, "COMPILE-ONLY");
    int ONLY = COLON(6, DOLIT, 0x40, LAST, AT, PSTOR, EXITT);
    HEADER(9, "IMMEDIATE");
    int IMMED = COLON(6, DOLIT, 0x80, LAST, AT, PSTOR, EXITT);
    int ENDD = IP;
    Serial.println();
    Serial.print("IP=");
    Serial.print(IP);
    Serial.print(" R-stack= ");
    Serial.print(popR << 2, HEX);
    IP = 0x180;
    int USER = LABEL(16, 6, EVAL, 0, 0, 0, 0, 0, 0, 0, 0x10, IMMED - 12, ENDD, IMMED - 12, INTER, EVAL, 0);

    // dump dictionary
    IP = 0;
    for (len = 0; len < 0x120; len++) {
        CheckSum();
    }

    // compile \data\load.txt
    if (!SPIFFS.begin(true)) {
        Serial.println("Error mounting SPIFFS");
    }
    File file = SPIFFS.open("/load.txt");
    if (file) {
        Serial.print("\n\nLoad file: ");
        len = file.read(cData + 0x8000, 0x7000);
        Serial.print(len);
        Serial.println(" bytes.");
        data[0x66] = 0; // >IN
        data[0x67] = len; // #TIB
        data[0x68] = 0x8000; // 'TIB
        P = 0x180; // EVAL
        WP = 0x184;
        evaluate();
        Serial.println(" Done loading.");
        file.close();
        SPIFFS.end();
    }
    // Setup web server handlers
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", index_html);
    });
    server.on("/input", HTTP_POST, handleInput);
    server.begin();

    printNetworkInfo();

    tft.begin();
    tft.setRotation(1);
    showNetworkInfo(tft);
}

void loop()
{
    server.handleClient();
}
