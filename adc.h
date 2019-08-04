#ifndef ADC_H
#define	ADC_H


#include "config.h"
#include "function.h"

unsigned int readADC() {
    ADCON0bits.GO = 1;
    __delay_us(50);
    while (!ADCON0bits.GODONE) {
    }
    return((ADRESH<<8)+ ADRESL);
}

#endif	