
#include <xc.h>
#include "config.h"
#include <stdio.h>

/*
 * File:   function.c
 * Author: Jonas Vargaski
 *
 * Created on 11 de Julho de 2018, 19:25
 */

//Delay em MS

void delay(int x) {
    while (x--) {
        __delay_us(1000);
        asm("CLRWDT"); // Reinicia WDT
    }
}

//Converte o n�mero de decimal para BCD

char DECtoBCD(char val) {
    char res;
    res = (val / 10) << 4;
    res = res | (val % 10);
    return res;
}

//Converte de BCD para decimal

char BCDtoDEC(char val) {
    return (val & 0x0F) + ((val & 0xF0) >> 4)*10;
}

//Converte numero de Decimal para Hexadecimal

int DECtoHEX(int hex) {

    return ((hex / 10) * 16) + (hex % 10);

}

//Converte um numero float em array de char com precisao 2 casas.

char* intToFloatStr(unsigned char valor) {
    int pr = valor >= 99 ? 0 : valor;
    unsigned char conv[4] = {' ', ' ', ' ', ' '};
    conv[0] = (char) ((pr / 10) + 48);
    conv[1] = (char) '.';
    conv[2] = (char) ((pr % 10) + 48);
    conv[3] = '\0'; // Para nao loquiar o lcd
    char end[4];
    sprintf(end, conv);
    return end;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    int a = x - in_min;
    int b = out_max - out_min;
    int c = in_max - in_min;
    long d = (long) a * b;
    long e = (long) d / c;
    int result = (int) e + out_min;
    if (result < out_min) return out_min;
    if (result > out_max) return out_max;
    return result;
}




