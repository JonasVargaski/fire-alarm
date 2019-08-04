
#ifndef TELA_PRINCIPAL_H
#define	TELA_PRINCIPAL_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"

void telaPrincipal() {
    if (pressao == 99) { // Se tiver leitura errada ou tirar o transdutor
        sprintf(line1, "- ERRO  TRANSDUTOR ");
        clearShiftREG();
    } else {
        sprintf(line1, " PRESSAO : %s bar", (intToFloatStr(pressao)));
    }
    if (status_estacionaria == ERRO) {
        falha = 1;
    } else if (status_jockey == 10) {
        falha = 6;
    } else if (status_principal == ERRO) {
        falha = 5;
    }
    if (falha != 0 || incendio == 1) {

        sprintf(line4, "Press ESC p/ Reset");
        if (btPress(b_esc)) {
            incendio = 0;
            status_estacionaria = OK;
            status_jockey = OK;
            status_principal = OK;
            tempo_limite_partida = 0;
            tempo_reenvio_SMS = 0;
            tempo_partida = 0;
            falha = 0;
            clearShiftREG();
        }
        switch (falha) {
            case 0:
                sprintf(line2, "     SISTEMA OK");
                break;
            case 1:
                sprintf(line2, "FALHA COMBUSTAO ");
                break;
            case 2:
                sprintf(line2, "FALHA RELOGIO ");
                break;
            case 3:
                sprintf(line2, "FALHA ELETRICA ");
                break;
            case 4:
                falha = 0; //PROXIMA FALHA
                break;
            case 5:
                sprintf(line2, "FALHA PRINCIPAL ");
                break;
            case 6:
                sprintf(line2, "FALHA JOCKEY ");
                break;
            default:
                falha = 0;
                break;
        }
    } else {
        sprintf(line2, "     SISTEMA OK");
        sprintf(line4, "  Press OK p/ MENU");
    }
    if (incendio) {
        sprintf(line3, " ALERTA DE INCENDIO");
    } else {
        sprintf(line3, " %02d:%02d:%02d %02d/%02d/%02d", _hor, _min, _sec, _dia, _mes, _ano);
    }

    if (btPress(b_ok)) {
        menu_posi++;
        sub_menu_posi = 1;
    }
}

#endif	

