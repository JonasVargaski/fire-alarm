#ifndef TRANSDUTOR_H
#define	TRANSDUTOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "adc.h"

void lerTransdutor() {
    char i = 0;
    float tensao = 0, _pressao = 0;
    for (i = 0; i < 9; i++) { // Ler o adc 10 vezes para filtrar erros.
        tensao += readADC()*0.004887586; // Converter para Volts
    }
    tensao = tensao / 10; // tira media de 10 leituras
    _tensao += (float) tensao;
    qtd_leitura++;

    if (tensao < 0.4) {
        pressao = -10;
    } else if (tensao > 4.5) { // Se estiver a leitura fora do range do transdutor o mesmo está errado.
        pressao = 99; // Se der erro as bombas nao vao ligar pois vai indicar pressao maxima
    } else if (qtd_leitura >= 6) {
        qtd_leitura = 0;
        tensao = (float) (_tensao / 6);
        _tensao = 0;
        _pressao = tensao - 0.5; // tira -0,5 de range do sensor
        _pressao = _pressao * 10 / 4; // converte para bar
        if (_pressao < 0) {
            _pressao = 0;
        }
        pressao = _pressao * 10; // passa pra numero inteiro para trabalhar com mais desempenho.
    }
}

#endif	

