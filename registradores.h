
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void configuraRegistradores() {
    //## ADC ENTRADAS
    ADCON0 = 0b10000001;
    ADCON1 = 0b11001110;
    TRISAbits.RA0 = 1;
    PORTAbits.AN0 = 0;

    //##Configura TIMER0
    T0CON = 0x81;
    INTCON = 0xE4;

    //CONFIGURAÇÃO LCD
    TRISDbits.RD5 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD0 = 0;

    //CONFIGURAÇÃO SERIAL
    TRISCbits.TRISC6 = 0; // configura TX como saida
    TRISCbits.TRISC7 = 1; // configura RX como entrada
    TXSTAbits.TXEN = 1; //Habilita Transmissao pela EUSART
    TXSTAbits.BRGH = 1; // modo velocidade alta
    TXSTAbits.SYNC = 0; // Modo assincrono
    RCSTAbits.SPEN = 1; // Configura pinos tx/rx
    RCSTAbits.CREN = 1; //Habilita Recepçao pela EUSART
    //9600 = 51  0.160% err @8mhz
    //19200 = 25  0.160% err @8mhz
    SPBRG = 25; // crystal 8mhz baudrate 19200bps
    INTCONbits.GIE = 1; // Habilita interrupçao global
    INTCONbits.PEIE = 1; // Habilita interrupçao por perifericos
    PIR1bits.RCIF = 0; // Limpa flag de recepçao uart
    PIE1bits.RCIE = 1; // Habilita interrupção por recepçao uart

    // Configuração inputs
    INTCONbits.RBIE = 0; // desativa interrupçao portrb
    TRISAbits.RA2 = 1; //INPUT sinal de entrada estacionario
    TRISAbits.RA5 = 1; //INPUT sinal de entrada aux 2 
    TRISCbits.RC0 = 1; //INPUT sinal de falta de energia 

    //Configuração TECLADO
    TRISBbits.RB2 = 1;
    TRISBbits.RB3 = 1;
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 1;

    //Configuração shift595
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    TRISEbits.RE2 = 0;
}
#endif	/* XC_HEADER_TEMPLATE_H */

