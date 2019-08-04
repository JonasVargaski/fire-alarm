#ifndef SHIFT595_H
#define SHIFT595_H

#include "config.h"
#include "variaveisGlobais.h"

//lacth
#define RCLK PORTEbits.RE1 
//clock
#define SRCLK PORTEbits.RE0
//data
#define SER  PORTEbits.RE2

#define rl_jockey 6      // OK
#define rl_principal 5    // OK
#define rl_motor_arranque 4  // OK
#define rl_sol_despressurizacao 2 // OK
#define rl_alarme 1  // OK
#define rl_sol_diesel 3 // OK
#define rl7 0
#define rl8 7


void shift_Init();
void setShiftREG();
void clearShiftREG();

unsigned char shift[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void clearShiftREG() {
    char i = 0;
    for (i = 0; i < 8; i++) {
        shift[i] = 0;
    }
}

void setShiftREG() {
    unsigned char i = 0;
    RCLK = 0;
    for (i = 0; i < 8; i++) {
        SRCLK = 0;
        SER = shift[i];
        SRCLK = 1;
    }
    RCLK = 1;
}

#endif
