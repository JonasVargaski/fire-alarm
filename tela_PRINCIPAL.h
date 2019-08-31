
#ifndef TELA_PRINCIPAL_H
#define	TELA_PRINCIPAL_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"
#include "function.h"
#include "shift595.h"

void resetParametros() {
    ocorrendoIncendio = false;
    codigoErro = 0;
    status_estacionaria = OK;
    status_jockey = OK;
    status_principal = OK;
    tempo_limite_partida = 0;
    timerReenvioSMS = 0;
    tempo_partida = 0;
    gsmOcupado = false;
    clearShiftREG();
}

void telaPrincipal() {

    sprintf(&line1[2], " %02d:%02d:%02d %02d/%02d", _hor, _min, _sec, _dia, _mes);

    line1[0] = ICONE_TORRE_SINAL;
    line1[1] = intensidadeSinal;
    line1[19] = ICONE_TOMADA;

    if (FALTA_ENERGIA) {
        blink(line1, 20, 20);
        //        codigoErro = 1;
    }

    switch (codigoErro) {
        case 1:
            sprintf(&line3[1], "*falta de energia");
            codigoErro = 0;
            break;
        case 3:
            sprintf(&line3[1], "*falha val. ladrao");
            break;
    }

    if (ocorrendoIncendio) {
        //        sprintf(line2, " ALERTA DE INCENDIO");
        sprintf(&line2[2], "Pressao %s BAR", intToFloatStr(pressao));
    }
    if (pressao > 99 || pressao < 0) { // Se tiver leitura errada ou tirar o transdutor
        sprintf(&line2[1], "Erro no Transdutor");
        clearShiftREG();
    } else {
        sprintf(&line2[2], "Pressao %s BAR", intToFloatStr(pressao));
    }


    if (codigoErro > 1 || ocorrendoIncendio) {
        blink(line3, 1, 19);
        shift[rl_alarme] = 1;
        sprintf(&line4[1], "Press ESC p/ Reset");
        if (btPress(b_esc)) {
            resetParametros();
        }
    }

    if (btPress(b_ok)) {
        menu_posi = 1; // entra no menu de ajustes
        sub_menu_posi = 1; // vai para a primeira posição do menu
    }
}

#endif	

