
#ifndef LOGS_H
#define LOGS_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "eeprom.h"

void salvarLOG() {
    char dtl_ultimo_registro = readEEPROM_ext(14);
    switch (dtl_ultimo_registro) {
        case 0:
            writeEEPROM_ext(60, _dia);
            writeEEPROM_ext(61, _mes);
            writeEEPROM_ext(62, _ano);
            writeEEPROM_ext(63, status_jockey);
            writeEEPROM_ext(64, status_principal);
            writeEEPROM_ext(65, status_estacionaria);
            writeEEPROM_ext(14, 1);
            break;
        case 1:
            writeEEPROM_ext(66, _dia);
            writeEEPROM_ext(67, _mes);
            writeEEPROM_ext(68, _ano);
            writeEEPROM_ext(69, status_jockey);
            writeEEPROM_ext(70, status_principal);
            writeEEPROM_ext(71, status_estacionaria);
            writeEEPROM_ext(14, 2);
            break;
        case 2:
            writeEEPROM_ext(72, _dia);
            writeEEPROM_ext(73, _mes);
            writeEEPROM_ext(74, _ano);
            writeEEPROM_ext(75, status_jockey);
            writeEEPROM_ext(76, status_principal);
            writeEEPROM_ext(77, status_estacionaria);
            writeEEPROM_ext(14, 0);
            break;
        default:
            writeEEPROM_ext(14, 0);
            break;

    }
}


#endif

