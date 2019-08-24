
#ifndef UTIL_H
#define	UTIL_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "eeprom.h"

void lerParametros() {
    char i = 0;
    char verifica_memoria_nova = 0;
    verifica_memoria_nova = readEEPROM_ext(12);
    if (verifica_memoria_nova != 20) { // Zera os numeros salvos na EEPROM caso Inserir uma nova
        for (i = 0; i < 100; i++) {
            writeEEPROM_ext(i, 0);
        }
        writeEEPROM_ext(12, 20);
    }

    sp_jockey = readEEPROM_ext(0);
    sp_principal = readEEPROM_ext(1);
    sp_estacionaria = readEEPROM_ext(2);
    horimetro = readEEPROM16_ext(8);
    sp_pressao_rede = readEEPROM_ext(4);
    intervalo_teste_dia = readEEPROM_ext(6);
    tempo_limite_estacionaria = readEEPROM_ext(7);
    dias_restante_teste = readEEPROM_ext(10);
    horario_de_teste = readEEPROM_ext(11);
    tempo_limite_teste_estacionaria = readEEPROM_ext(13);
    // 14 reservado
    habilitaTesteAutomatico = readEEPROM_ext(15);

    for (i = 0; i < 11; i++) {
        tel1[i] = readEEPROM_ext(20 + i);
    }
    for (i = 0; i < 11; i++) {
        tel2[i] = readEEPROM_ext(40 + i);
    }
}

#endif	

