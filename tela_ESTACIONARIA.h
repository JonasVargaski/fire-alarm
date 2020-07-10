
#ifndef TELA_ESTACIONARIA_H
#define	TELA_ESTACIONARIA_H

#include <xc.h>

#include "teclado.h"
#include "eeprom.h" // include processor files - each processor file is guarded.  

void configurarEstacionaria() {

    sprintf(line1, "Configuracao tempo");
    sprintf(line2, "Max. PARTIDA:");
    sprintf(&line2[16], "%3ds", tempo_limite_estacionaria); // tempo maximo em que a bomba tenta ligar antes de dar erro.
    sprintf(line3, "Max. TESTE:");
    sprintf(&line3[16], "%3ds", tempo_limite_teste_estacionaria); // tempo em que a bomba permanece ligada durante os testes
    sprintf(line4, "horimetro:%5d min", horimetro);

    switch (option_posi) {
        case 0:
            blink(line2, 16, 20);
            if (btPress(b_mais)) {
                tempo_limite_estacionaria++;
            }
            if (btPress(b_menos)) {
                tempo_limite_estacionaria--;
            }
            break;
        case 1:
            blink(line3, 16, 20);
            if (btPress(b_mais)) {
                tempo_limite_teste_estacionaria++;
            }
            if (btPress(b_menos)) {
                tempo_limite_teste_estacionaria--;
            }
            break;
        case 2:
            blink(line4, 11, 20);
            if (btPress(b_menos)) {
                horimetro = 0;
                writeEEPROM_ext(8, 0);
            }
            break;
        default:option_posi = 0;
            break;
    }

    //#CONTROLE
    if (btPress(b_ok)) {
        option_posi++;
    } else if (btPress(b_esc)) {
        writeEEPROM_ext(7, tempo_limite_estacionaria);
        writeEEPROM_ext(13, tempo_limite_teste_estacionaria);
        option_posi = 0;
        menu_posi = _menu_posi;
    }
}

#endif	