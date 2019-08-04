#ifndef SERIAL_H
#define	SERIAL_H

#include "config.h"
#include "variaveisGlobais.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "function.h"


unsigned char bufferSerial[60];
unsigned char bufferAUX[20];
unsigned char cbf = 0;

void putch(char c) {
    while (!TXSTAbits.TRMT);
    TXREG = c;
}

void writeChar(char *hex) {
    while (!TXSTAbits.TRMT);
    TXREG = hex;
}

bool aguardaBuffer(char *str) {
    if (strstr(bufferSerial, str) != NULL) {
        delay(150);
        return true;
    }
    return false;
}

void limparBuffer() {
    cbf = 0;
    memset(bufferSerial, 0, sizeof (bufferSerial));
    delay(150);
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

