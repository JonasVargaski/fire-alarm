/*
 * File:   I2C.c
 * Author: Jonas Vargaski
 *
 * Created on 11 de Julho de 2018, 18:52
 */

#include "config.h"
#include "function.h"



void I2C_Master_Init(const unsigned long c) {
    SSPCON1 = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ / (4 * c)) - 1;
    SSPSTAT = 0;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
}

void I2C_Wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Start() {
    I2C_Wait();
    SEN = 1;
}

void I2C_Restart() {
    I2C_Wait();
    RSEN = 1;
}

void I2C_Stop() {
    I2C_Wait();
    PEN = 1;

}

void I2C_Write(unsigned d) {
    I2C_Wait();
    SSPBUF = d;
}

unsigned short I2C_Read(unsigned short a) {
    unsigned short temp;
    I2C_Wait();
    RCEN = 1;
    I2C_Wait();
    temp = SSPBUF;
    I2C_Wait();
    ACKDT = (a) ? 0 : 1;
    ACKEN = 1;
    return temp;
}

