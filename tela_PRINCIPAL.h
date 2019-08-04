
#ifndef TELA_PRINCIPAL_H
#define	TELA_PRINCIPAL_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"
#include "function.h"

void telaPrincipal() {

    line1[0] = ICONE_TORRE_SINAL;
    line1[1] = intensidadeSinal;
    line1[19] = ICONE_TOMADA;

    if (FALTA_ENERGIA) {
        blink(line1, 19, 20);
    }

    if (pressao == 99) { // Se tiver leitura errada ou tirar o transdutor
        sprintf(line2, "- ERRO  TRANSDUTOR");
        clearShiftREG();
    } else {
        sprintf(&line3[2], "Pressao %s bar", (intToFloatStr(pressao)));
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
    }
    if (incendio) {
        sprintf(line3, " ALERTA DE INCENDIO");
    }

    sprintf(line4, " %02d:%02d:%02d %02d/%02d/%02d", _hor, _min, _sec, _dia, _mes, _ano);

    if (btPress(b_ok)) {
        menu_posi++;
        sub_menu_posi = 1;
    }
}

#endif	

