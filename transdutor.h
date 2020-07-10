#ifndef TRANSDUTOR_H
#define	TRANSDUTOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "function.h"

void lerTransdutor() {
    char i = 0;
    int mediaLeitura = 0;
    int adc = 0;
    int _pressao = 0;

        ADCON0bits.CHS = 0b000;
    for (i = 0; i < 15; i++) { // Ler o adc 10 vezes para filtrar erros.
        adc = 0;
        __delay_us(30);
        ADCON0bits.GO = 1;
        while (!ADCON0bits.GODONE);
        adc = ((ADRESH << 8) + ADRESL);
        mediaLeitura += map(adc, 102, 920, 0, 100);
        if (adc < 50 && timerIntervaloEntreBombas == 0) {
            flagErroTransdutor = true;
        }
    }

    coletaMedias[posicaoColetaLeitura] = (int) mediaLeitura / 15; // passa pra numero inteiro para trabalhar com mais desempenho. 

    if (posicaoColetaLeitura >= 5) {
        posicaoColetaLeitura = 0;
    } else {
        posicaoColetaLeitura++;
    }

    for (i = 0; i < 5; i++) {
        _pressao += coletaMedias[i];
    }

    pressao = (int) _pressao / 5;
}

#endif	

