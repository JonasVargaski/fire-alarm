
#ifndef TELA_TESTE_H
#define	TELA_TESTE_H

#include <xc.h> 
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>

void telaTeste() {
    line1[0] = ICONE_TORRE_SINAL;
    line1[1] = intensidadeSinal;
    sprintf(&line1[3], "%s", bufferSerial);
    sprintf(line2, "etapa %d", etapaComunicacao);
    sprintf(line3, "tetapa %d ", timerEtapaComunicacao);
    sprintf(line4, "tsinal %d", timerVerificarSinal);
}


#endif	
