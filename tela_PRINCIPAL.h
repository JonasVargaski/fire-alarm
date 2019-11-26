
#ifndef TELA_PRINCIPAL_H
#define	TELA_PRINCIPAL_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"
#include "function.h"
#include "shift595.h"
#include "comunicacao.h"

void resetParametros() {
    ocorrendoIncendio = false;
    codigoErro = 0;
    status_estacionaria = OK;
    status_jockey = OK;
    status_principal = OK;
    gsmOcupado = false;
    shift[rl_alarme] = 0;
    tempo_limite_partida = 0;
    tempo_partida = 0;
    clearShiftREG();
}

void telaPrincipal() {

    sprintf(&line1[2], " %02d:%02d:%02d %02d/%02d", _hor, _min, _sec, _dia, _mes);

    line1[0] = ICONE_TORRE_SINAL;
    line1[1] = intensidadeSinal;
    line1[19] = ICONE_TOMADA;

    switch (codigoErro) {
        case 1:
            sprintf(line4, " *falha eletrica");
            break;
        case 2:
            sprintf(line4, " *falha GSM");
            break;
        case 3:
            sprintf(line4, " *falha val. ladrao");
            shift[rl_alarme] = 1;
            break;
        case 4:
            sprintf(line4, " *falha B. jockey");
            shift[rl_alarme] = 1;
            break;
        case 5:
            sprintf(line4, " *falha B. principal");
            shift[rl_alarme] = 1;
            break;
        case 6:
            sprintf(line4, " *falha B. combustao");
            shift[rl_alarme] = 1;
            break;
    }

    if (ocorrendoIncendio) {
        sprintf(line2, " ALERTA DE INCENDIO");
    }

    if (flagErroTransdutor) { // Se tiver leitura errada ou tirar o transdutor
        clearShiftREG();
        sprintf(&line3[1], "Erro no Transdutor");
        ocorrendoIncendio = false;
        shift[rl_alarme] = 1; // liga alarme sinalizando erro.
    } else {
        sprintf(&line3[2], "Pressao %s BAR", intToFloatStr(pressao));
    }


    if (codigoErro > 1 || ocorrendoIncendio) {
        if (flagTexto) {
            sprintf(line4, " Press ESC p/ Reset");
        }
        if (btPress(b_esc)) {
            resetParametros();
        }
    }

    if (btPress(b_ok)) {
        menu_posi = 1; // entra no menu de ajustes
        sub_menu_posi = 1; // vai para a primeira posição do menu
    }


    if (FALTA_ENERGIA) {
        blink(line1, 20, 20);
        if (codigoErro == 0) {
            codigoErro = 1;
        }
    }
    if (status_jockey == ERRO) {
        codigoErro = 4;
    } else if (status_principal == ERRO) {
        codigoErro = 5;
    } else if (status_estacionaria == ERRO) {
        codigoErro = 6;
    }
}

#endif	

