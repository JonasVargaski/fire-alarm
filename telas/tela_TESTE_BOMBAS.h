
#ifndef TELA_TESTE_BOMBAS_H
#define	TELA_TESTE_BOMBAS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"

void tela_testeBombas() {

    sprintf(line1, "# PROXIMO EM %2d dias", (intervalo_teste_dia - dias_restante_teste));
    sprintf(line2, " PERIODO: %2d dias", intervalo_teste_dia);
    sprintf(line3, " HORARIO: %02d horas", horario_de_teste);
    sprintf(line4, " INICIAR TESTE");

    if (btPress(b_esc)) {
        writeEEPROM_ext(6, intervalo_teste_dia);
        writeEEPROM_ext(11, horario_de_teste);
        menu_posi = _menu_posi;
    }
    if (btPress(b_ok)) {
        option_posi++;
    }
    switch (option_posi) {
        case 0:
            line2[0] = '>';
            if (btPress(b_mais)) {
                intervalo_teste_dia++;
            }
            if (btPress(b_menos)) {
                intervalo_teste_dia--;
            }
            break;
        case 1:
            line3[0] = '>';
            if (btPress(b_mais)) {
                horario_de_teste++;
            }
            if (btPress(b_menos)) {
                horario_de_teste--;
            }
            if (horario_de_teste > 23 || horario_de_teste < 0) {
                horario_de_teste = 0;
            }
            break;
        case 2:
            line4[0] = '>';
            if (btPress(b_mais)) {
                if (!incendio) {
                    menu_posi = 28;
                    teste_run = 2;
                }
            }
            break;
        default:option_posi = 0;
            break;
    }
}

#endif	

