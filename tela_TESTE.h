
#ifndef TELA_TESTE_H
#define	TELA_TESTE_H

#include <xc.h> 
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>

void telaTeste() {
    sprintf(line1, "TELA TESTE");
    delay(4000);
    printf("AT+CSQ\r\n");

    if (aguardaBuffer("+CSQ: ")) {
        int sinal = atoi(getSerial("+CSQ: ", ','));
        if (sinal >= 10 && sinal <= 14) {;
            return ICONE_SINAL_FRACO;
        } else if (sinal >= 16 && sinal <= 19) {
            return ICONE_SINAL_MEDIO;
        } else if (sinal >= 20 && sinal <= 30) {
            return ICONE_SINAL_BOM;
        }
        sprintf(line3, "Sinal: %d", sinal);
        limparBuffer();
    }
}


#endif	
