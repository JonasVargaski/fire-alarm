#ifndef SERIAL_H
#define	SERIAL_H

#include "config.h"
#include "variaveisGlobais.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


unsigned char bufferSerial[80];
unsigned char bufferAUX[20];

void putch(char c) {
    while (!TXSTAbits.TRMT);
    TXREG = c;
}

bool aguardaBuffer(char *str) {
    return strstr(bufferSerial, str) != NULL;
}

void limparBuffer() {
    memset(bufferSerial, 0, sizeof (bufferSerial));
}

void tratarSerial(c) {
    sprintf(bufferSerial, "%s%c", bufferSerial, c);
}

char* getSerial(char * str, char end) {
    memset(bufferAUX, 0, sizeof (bufferAUX));
    unsigned int length = strlen(str);
    char* ret;
    ret = strstr(bufferSerial, str);
    if (ret) {
        unsigned char i = 0;
        for (i = length; i < strlen(ret); i++) {
            if (ret[i] == end) {
                sprintf(bufferAUX, "%s%c", bufferAUX, '\0');
                return bufferAUX;
            }
            sprintf(bufferAUX, "%s%c", bufferAUX, ret[i]);
        }
    }
    return 0;
}

#endif

