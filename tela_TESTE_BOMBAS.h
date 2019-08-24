
#ifndef TELA_TESTE_BOMBAS_H
#define	TELA_TESTE_BOMBAS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"

void tela_testeBombas() {

    sprintf(line1, "#Proximo em %2d dias", (intervalo_teste_dia - dias_restante_teste));
    sprintf(line2, "INTERVALO: %2d dias", intervalo_teste_dia);
    sprintf(line3, "HORARIO: %02d horas", horario_de_teste);
    sprintf(line4, "ATIVAR: %s", habilitaTesteAutomatico == 0 ? "Nao" : "Sim");

    if (btPress(b_esc)) {
        writeEEPROM_ext(6, intervalo_teste_dia);
        writeEEPROM_ext(11, horario_de_teste);
        writeEEPROM_ext(15, habilitaTesteAutomatico);
        menu_posi = _menu_posi;
    }
    if (btPress(b_ok)) {
        option_posi++;
    }
    switch (option_posi) {
        case 0:
            blink(line2, 11, 14);
            if (btPress(b_mais)) {
                intervalo_teste_dia++;
            }
            if (btPress(b_menos)) {
                intervalo_teste_dia--;
            }
            break;
        case 1:
            blink(line3, 9, 11);
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
            blink(line4, 9, 11);
            if (btPress(b_mais)) {
                habilitaTesteAutomatico = 1;
            }
            if (btPress(b_menos)) {
                habilitaTesteAutomatico = 0;
            }
            break;
        default:option_posi = 0;
            break;
    }
}

#endif	

