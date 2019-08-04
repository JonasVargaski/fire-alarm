
#ifndef TELA_DATALOGER_H
#define	TELA_DATALOGER_H

#include <xc.h> // include processor files - each processor file is guarded.  

void dataLoger() {
    sprintf(line1, "#%d DATA: %02d/%02d/%02d", dtl_pos, dtl_dia, dtl_mes, dtl_ano);
    sprintf(line2, "JOKEY - %s ", dtl_jockey == 1 ? "OK" : "ERRO");
    sprintf(line3, "PRINCIPAL - %s ", dtl_principal == 1 ? "OK" : "ERRO");
    sprintf(line4, "COMBUSTAO - %s ", dtl_estacionaria == 1 ? "OK" : "ERRO");

    if (btPress(b_esc)) {
        option_posi = 0;
        menu_posi = _menu_posi;
    }
    if (btPress(b_menos)) {
        dtl_pos--;
        dtl_visualiza_log = 1;
    }
    if (btPress(b_mais)) {
        dtl_pos++;
        dtl_visualiza_log = 1;
    }
    if (dtl_visualiza_log == 1) { // EVITA FICAR FAZENDO LEITURAS NA EEPROM.
        switch (dtl_pos) {
            case 0:
                dtl_dia = readEEPROM_ext(60);
                dtl_mes = readEEPROM_ext(61);
                dtl_ano = readEEPROM_ext(62);
                dtl_jockey = readEEPROM_ext(63);
                dtl_principal = readEEPROM_ext(64);
                dtl_estacionaria = readEEPROM_ext(65);
                dtl_visualiza_log = 0;
                break;
            case 1:
                dtl_dia = readEEPROM_ext(66);
                dtl_mes = readEEPROM_ext(67);
                dtl_ano = readEEPROM_ext(68);
                dtl_jockey = readEEPROM_ext(69);
                dtl_principal = readEEPROM_ext(70);
                dtl_estacionaria = readEEPROM_ext(71);
                dtl_visualiza_log = 0;
                break;
            case 2:
                dtl_dia = readEEPROM_ext(72);
                dtl_mes = readEEPROM_ext(73);
                dtl_ano = readEEPROM_ext(74);
                dtl_jockey = readEEPROM_ext(75);
                dtl_principal = readEEPROM_ext(76);
                dtl_estacionaria = readEEPROM_ext(77);
                dtl_visualiza_log = 0;
                break;
            default: dtl_pos = 0;
                dtl_visualiza_log = 1;
                break;
        }
    }
}


#endif	

