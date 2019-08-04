
#ifndef TELA_RELOGIO_H
#define	TELA_RELOGIO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"


void tela_ajusteRTC() {
    sprintf(line1, "HORARIO");
    sprintf(line2, " %02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec);
    sprintf(line3, "DATA");
    sprintf(line4, " %02d/%02d/%02d", rtc.date, rtc.month, rtc.year);

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
            line2[0] = '>';
            if (btPress(b_mais)) {
                rtc.hour++;
            }
            if (btPress(b_menos)) {
                rtc.hour--;
            }
            break;
        case 2:
            line2[3] = '>';
            if (btPress(b_mais)) {
                rtc.min++;
            }
            if (btPress(b_menos)) {
                rtc.min--;
            }
            break;
        case 3:
            line2[6] = '>';
            if (btPress(b_mais)) {
                rtc.sec++;
            }
            if (btPress(b_menos)) {
                rtc.sec--;
            }
            break;
        case 4:
            line4[0] = '>';
            if (btPress(b_mais)) {
                rtc.date++;
            }
            if (btPress(b_menos)) {
                rtc.date--;
            }
            break;
        case 5:
            line4[3] = '>';
            if (btPress(b_mais)) {
                rtc.month++;
            }
            if (btPress(b_menos)) {
                rtc.month--;
            }
            break;
        case 6:
            line4[6] = '>';
            if (btPress(b_mais)) {
                rtc.year++;
            }
            if (btPress(b_menos)) {
                rtc.year--;
            }
            break;
        default:option_posi = 1;
            break;
    }
    validaRTC();
}



#endif	

