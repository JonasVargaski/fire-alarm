
#ifndef LOGS_H
#define LOGS_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "eeprom.h"

void salvarLOG() {
    unsigned int ultimoLogSalvo = readEEPROM_ext(14);
    if (ultimoLogSalvo < 60 || ultimoLogSalvo > 250) { // salva os logs na posicao 60 pra cima da eeprom
        ultimoLogSalvo = 60;
    }
    writeEEPROM_ext(ultimoLogSalvo, _dia);
    writeEEPROM_ext(++ultimoLogSalvo, _mes);
    writeEEPROM_ext(++ultimoLogSalvo, _ano);
    writeEEPROM_ext(++ultimoLogSalvo, _hor);
    writeEEPROM_ext(++ultimoLogSalvo, _min);
    writeEEPROM_ext(++ultimoLogSalvo, status_jockey);
    writeEEPROM_ext(++ultimoLogSalvo, status_principal);
    writeEEPROM_ext(++ultimoLogSalvo, status_estacionaria);

    writeEEPROM_ext(14, ++ultimoLogSalvo);
}

void apagarLOGS() {
    char b = 0;
    writeEEPROM_ext(14, 60);
    for (b = 60; b < 250; b++) {
        writeEEPROM_ext(b, 0);
    }
}


#endif

