#ifndef TELA_CONFIG_PRESSAO_H
#define	TELA_CONFIG_PRESSAO_H

#include <xc.h>
#include "variaveisGlobais.h"

void ajusteSP_Bombas() {

    sprintf(line1, "GERAL : %s", intToFloatStr(sp_pressao_rede));
    sprintf(line2, "JOKEY : %s", intToFloatStr(sp_jockey));
    sprintf(line3, "PRINCIPAL : %s", intToFloatStr(sp_principal));
    sprintf(line4, "COMBUSTAO : %s", intToFloatStr(sp_estacionaria));

    if (btPress(b_esc)) {
        writeEEPROM_ext(0, sp_jockey);
        writeEEPROM_ext(1, sp_principal);
        writeEEPROM_ext(2, sp_estacionaria);
        writeEEPROM_ext(4, sp_pressao_rede);
        option_posi = 0;
        menu_posi = _menu_posi;
    }

    if (btPress(b_ok)) {
        option_posi++;
    }

    switch (option_posi) {
        case 0:
            blink(line1, 8, 15);
            if (btPress(b_mais)) {
                sp_pressao_rede++;
            }
            if (btPress(b_menos)) {
                sp_pressao_rede--;
            }
            if (sp_pressao_rede > 99) {
                sp_pressao_rede = 0;
            }
            break;
        case 1:
            blink(line2, 8, 15);
            if (btPress(b_mais)) {
                sp_jockey++;
            }
            if (btPress(b_menos)) {
                sp_jockey--;
            }
            if (sp_jockey > 99) {
                sp_jockey = 0;
            }
            break;
        case 2:
            blink(line3, 12, 18);
            if (btPress(b_mais)) {
                sp_principal++;
            }
            if (btPress(b_menos)) {
                sp_principal--;
            }
            if (sp_principal > 99) {
                sp_principal = 0;
            }
            break;
        case 3:
            blink(line4, 12, 18);
            if (btPress(b_mais)) {
                sp_estacionaria++;
            }
            if (btPress(b_menos)) {
                sp_estacionaria--;
            }
            if (sp_estacionaria > 99) {
                sp_estacionaria = 0;
            }
            break;
        default: option_posi = 0;
            break;
    }
}

#endif	

