
#ifndef TELA_ESTACIONARIA_H
#define	TELA_ESTACIONARIA_H

#include <xc.h> // include processor files - each processor file is guarded.  

void configurarEstacionaria() {
    sprintf(line1, " TMP LIMT PART: %3ds", tempo_limite_estacionaria);
    sprintf(line2, " TMP LIMT TEST: %3ds", tempo_limite_teste_estacionaria); // tempo em que a bomba permanece ligada durante os testes
    sprintf(line4, "#HORIMETRO:%5d min", horimetro);

    if (btPress(b_ok)) {
        option_posi++;
    }
    if (btPress(b_esc)) {
        option_posi = 0;
        writeEEPROM_ext(7, tempo_limite_estacionaria);
        writeEEPROM_ext(13, tempo_limite_teste_estacionaria);
        menu_posi = _menu_posi;
    }
    switch (option_posi) {
        case 0:
            line1[0] = '>';
            if (btPress(b_mais)) {
                tempo_limite_estacionaria++;
            }
            if (btPress(b_menos)) {
                tempo_limite_estacionaria--;
            }
            break;
        case 1:
            line2[0] = '>';
            if (btPress(b_mais)) {
                tempo_limite_teste_estacionaria++;
            }
            if (btPress(b_menos)) {
                tempo_limite_teste_estacionaria--;
            }
            break;
        default:option_posi = 0;
            break;
    }
}

#endif	