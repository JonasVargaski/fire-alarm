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


bool flag_b_esc, _b_esc = false;
bool flag_b_menos, _b_menos = false;
bool flag_b_mais, _b_mais = false;
bool flag_b_ok, _b_ok = false;
bool isPressed = false;
char counterPressedTime = 0;
//Cnfigura os botoes, setar os tris.

void readButtons() {
    if (!PORTBbits.RB2) {
        flag_b_esc = true;
        if (isPressed) {
            _b_esc = true;
        }
    } else if (PORTBbits.RB2 && flag_b_esc) {
        _b_esc = true;
        flag_b_esc = false;
    }
    //////////////////////////////////////////////
    if (!PORTBbits.RB4) {
        flag_b_menos = true;
        if (isPressed) {
            _b_menos = true;
        }
    } else if (PORTBbits.RB4 && flag_b_menos) {
        _b_menos = true;
        flag_b_menos = false;
    }
    //////////////////////////////////////////////////
    if (!PORTBbits.RB5) {
        flag_b_mais = true;
        if (isPressed) {
            _b_mais = true;
        }
    } else if (PORTBbits.RB5 && flag_b_mais) {
        _b_mais = true;
        flag_b_mais = false;
    }
    //////////////////////////////////////////////////
    if (!PORTBbits.RB3) {
        flag_b_ok = true;
        if (isPressed) {
            _b_ok = true;
        }
    } else if (PORTBbits.RB3 && flag_b_ok) {
        _b_ok = true;
        flag_b_ok = false;
    }

    if (!PORTBbits.RB2 || !PORTBbits.RB3 || !PORTBbits.RB4 || !PORTBbits.RB5) {
        counterPressedTime++;
        if (counterPressedTime >= 8) {
            counterPressedTime = 8; // 800 mili segundos
            isPressed = true;
        }
    } else {
        isPressed = false;
        counterPressedTime = 0;
    }
}

char btPress(char bt) {
    switch (bt) {
        case b_esc: if (_b_esc) {
                _b_esc = false;
                if (!isPressed) {
                    timerAtrasoLCD = 1;
                }
                return true;
            }
            return false;
        case b_mais:
            if (_b_mais) {
                _b_mais = false;
                if (!isPressed) {
                    timerAtrasoLCD = 1;
                }
                return true;
            }
            return false;
        case b_menos: if (_b_menos) {
                _b_menos = false;
                if (!isPressed) {
                    timerAtrasoLCD = 1;
                }
                return true;
            }
            return false;
        case b_ok: if (_b_ok) {
                _b_ok = false;
                if (!isPressed) {
                    timerAtrasoLCD = 1;
                }
                return true;
            }
        default: return false;
    }
}

#endif