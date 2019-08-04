
#ifndef TELA_RELOGIO_H
#define	TELA_RELOGIO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "lcd.h"
#include "teclado.h"
#include "ds1307.h"

void tela_ajusteRTC() {
    sprintf(line1, "DATA:");
    sprintf(line2, "%02d/%02d/%02d", rtc.date, rtc.month, rtc.year);
    sprintf(&line1[11], "HORA:");
    sprintf(&line2[11], "%02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec);

    if (btPress(b_ok)) {
        ajst_rtc = 1; // seta flag que nao deixa a funçao timer alterar o horario.
        option_posi++;
    }
    if (btPress(b_esc)) {
        ajst_rtc = 0;
        option_posi = 0;
        setDS1307TimeDate(&rtc);
        menu_posi = _menu_posi;
        _min++; // Adianta 1 minuto somente para quando sair da tela sincronizar
    }
    switch (option_posi) {
        case 1:
            blink(line2, 1, 2);
            if (btPress(b_mais)) {
                rtc.date++;
            }
            if (btPress(b_menos)) {
                rtc.date--;
            }
            break;
        case 2:
            blink(line2, 4, 5);
            if (btPress(b_mais)) {
                rtc.month++;
            }
            if (btPress(b_menos)) {
                rtc.month--;
            }
            break;
        case 3:
            blink(line2, 7, 8);
            if (btPress(b_mais)) {
                rtc.year++;
            }
            if (btPress(b_menos)) {
                rtc.year--;
            }
            break;
        case 4:
            blink(line2, 12, 13);
            if (btPress(b_mais)) {
                rtc.hour++;
            }
            if (btPress(b_menos)) {
                rtc.hour--;
            }
            break;
        case 5:
            blink(line2, 15, 16);
            if (btPress(b_mais)) {
                rtc.min++;
            }
            if (btPress(b_menos)) {
                rtc.min--;
            }
            break;
        case 6:
            blink(line2, 18, 19);
            if (btPress(b_mais)) {
                rtc.sec++;
            }
            if (btPress(b_menos)) {
                rtc.sec--;
            }
            break;
        default:option_posi = 1;
            break;
    }
    validaRTC();
}



#endif	

