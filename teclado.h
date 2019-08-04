#ifndef TECLADO_H
#define TECLADO_H

#include <stdbool.h>
#include "variaveisGlobais.h"

#define b_esc 1
#define b_menos 2
#define b_mais 3
#define b_ok 4

void readButtons(void);
char btPress(char bt);
//bool btPress(bool bt);

bool bt_press = true;
bool flag_b_esc, _b_esc = false;
bool flag_b_menos, _b_menos = false;
bool flag_b_mais, _b_mais = false;
bool flag_b_ok, _b_ok = false;

//Cnfigura os botoes, setar os tris.


void readButtons() {
    if (!PORTBbits.RB2) {
        flag_b_esc = true;
    }
    if (PORTBbits.RB2 && flag_b_esc) {
        _b_esc = true;
        flag_b_esc = false;
    }
    //////////////////////////////////////////////
    if (!PORTBbits.RB4) {
        flag_b_menos = true;
    }
    if (PORTBbits.RB4 && flag_b_menos) {
        _b_menos = true;
        flag_b_menos = false;
    }
    //////////////////////////////////////////////////
    if (!PORTBbits.RB5) {
        flag_b_mais = true;
    }
    if (PORTBbits.RB5 && flag_b_mais) {
        _b_mais = true;
        flag_b_mais = false;
    }
    //////////////////////////////////////////////////
    if (!PORTBbits.RB3) {
        flag_b_ok = true;
    }
    if (PORTBbits.RB3 && flag_b_ok) {
        _b_ok = true;
        flag_b_ok = false;
    }
}

char btPress(char bt) {
    switch (bt) {
        case b_esc: if (_b_esc) {
                _b_esc = false;
                bt_press = true; // sempre que for pressionado um botao, limpa a tela
                return 1;
            }
            return false;
        case b_mais: if (_b_mais) {
                _b_mais = false;
                 bt_press = true;
                return 1;
            }
            return false;
        case b_menos: if (_b_menos) {
                _b_menos = false;
                 bt_press = true;
                return 1;
            }
            return false;
        case b_ok: if (_b_ok) {
                _b_ok = false;
                 bt_press = true;
                return 1;
            }
            return 0;
        default: return 0;
    }
}

#endif