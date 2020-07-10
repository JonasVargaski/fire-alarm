
#ifndef TELA_DATALOGER_H
#define	TELA_DATALOGER_H

#include <xc.h>
#include "teclado.h"
#include "logs.h"

unsigned char dtl_posicao = 60, dtl_dia = 0, dtl_mes = 0, dtl_ano = 0, dtl_hora = 0, dtl_min = 0, dtl_jockey = 0, dtl_principal = 0, dtl_estacionaria = 0, dtl_carregar_log = 0;
unsigned char totalLogs = 0, dtl_posi_visualizar = 0;

void dataLoger() {
    sprintf(line1, "Data: %02d/%02d/%02d %02d:%02d", dtl_dia, dtl_mes, dtl_ano, dtl_hora, dtl_min);
    sprintf(line2, "Jokey: %s", dtl_jockey == 1 ? "OK" : "ERRO");
    sprintf(&line2[14], "#%02d/%02d", (dtl_posi_visualizar + 1), totalLogs);
    sprintf(line3, "Principal: %s", dtl_principal == 1 ? "OK" : "ERRO");
    sprintf(line4, "Combustao: %s", dtl_estacionaria == 1 ? "OK" : "ERRO");


    if (dtl_carregar_log == 1) { // EVITA FICAR FAZENDO LEITURAS NA EEPROM.
        char logs = readEEPROM_ext(14);
        totalLogs = (logs - 60) / 8;

        dtl_posi_visualizar = (dtl_posicao - 60) / 8;

        dtl_dia = readEEPROM_ext(dtl_posicao);
        dtl_mes = readEEPROM_ext((dtl_posicao + 1));
        dtl_ano = readEEPROM_ext((dtl_posicao + 2));
        dtl_hora = readEEPROM_ext((dtl_posicao + 3));
        dtl_min = readEEPROM_ext((dtl_posicao + 4));
        dtl_jockey = readEEPROM_ext((dtl_posicao + 5));
        dtl_principal = readEEPROM_ext((dtl_posicao + 6));
        dtl_estacionaria = readEEPROM_ext((dtl_posicao + 7));
        dtl_carregar_log = 0;
    }
    //#CONTROLE
    if (btPress(b_esc)) {
        option_posi = 0;
        menu_posi = _menu_posi;
    } else if (btPress(b_menos)) {
        dtl_posicao -= 8;
        if (dtl_posicao < 60) {
            dtl_posicao = 60;
        }
        dtl_carregar_log = 1;
    } else if (btPress(b_mais)) {
        if ((dtl_posi_visualizar + 1) >= totalLogs) {
            return;
        }
        dtl_posicao += 8;
        dtl_carregar_log = 1;
    }
}


#endif	

