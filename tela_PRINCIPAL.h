
#ifndef TELA_PRINCIPAL_H
#define	TELA_PRINCIPAL_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"
#include "function.h"
#include "comunicacao.h"
#include "lcd.h"

void resetParametros() {
    timerIntervaloEntreBombas = 3;
    ocorrendoIncendio = 0;
    codigoErro = 0;
    status_estacionaria = OK;
    status_jockey = OK;
    status_principal = OK;
    gsmOcupado = false;
    out_RL_PRINCIPAL = 0;
    out_RL_JOCKEY = 0;
    out_RL_ALARME = 0;
    out_BOMBA_ESTACIONARIA = 0;
    tempo_limite_partida = 0;
    tempo_partida = 0;
    flagErroTransdutor = false;    
    reiniciaLCD();
}

void telaPrincipal() {

    sprintf(&line1[4], " %02d:%02d %02d/%02d", _hor, _min, _dia, _mes);

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
            out_RL_ALARME = 1;
            break;
        case 4:
            sprintf(line4, " *falha B. jockey");
            out_RL_ALARME = 1;
            break;
        case 5:
            sprintf(line4, " *falha B. principal");
            out_RL_ALARME = 1;
            break;
        case 6:
            sprintf(line4, " *falha B. combustao");
            out_RL_ALARME = 1;
            break;
    }

    if (ocorrendoIncendio == 1) {
        sprintf(line2, " ALERTA DE INCENDIO");
    }

    if (flagErroTransdutor) { // Se tiver leitura errada ou tirar o transdutor
        sprintf(&line3[1], "Erro no Transdutor");
        ocorrendoIncendio = 0;
        out_RL_ALARME = 1;
        out_RL_PRINCIPAL = 0;
        out_RL_JOCKEY = 0;
        out_BOMBA_ESTACIONARIA = 0;

    } else {
        sprintf(&line3[2], "Pressao %s BAR", intToFloatStr(pressao));
    }

    if (codigoErro > 1 || ocorrendoIncendio == 1) {
        if (textoAlternativo) {
            sprintf(line4, " Press ESC p/ Reset");
        }
    }

    if (btPress(b_esc)) {
        resetParametros();
    } else if (btPress(b_ok)) {
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

