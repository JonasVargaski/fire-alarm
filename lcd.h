#ifndef LCD_H
#define	LCD_H

#include "config.h"
#include "variaveisGlobais.h"

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char* a);
void Lcd_Set_Cursor(char a, char b);

void Lcd_Init() {

#define RS PORTDbits.RD1
#define EN PORTDbits.RD0
#define D4 PORTDbits.RD2
#define D5 PORTDbits.RD3
#define D6 PORTDbits.RD4
#define D7 PORTDbits.RD5

    Lcd_Port(0x00);
    __delay_ms(30);
    Lcd_Cmd(0x03);
    __delay_ms(15);
    Lcd_Cmd(0x03);
    __delay_ms(20);
    Lcd_Cmd(0x03);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06);
}

void Lcd_Port(char a) {
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(15);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    switch (a) {
        case 1:
            temp = 0x80 + b - 1;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 2:
            temp = 0xC0 + b - 1;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 3:
            temp = 0x94 + b;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        case 4:
            temp = 0xD4 + b;
            z = temp >> 4;
            y = temp & 0x0F;
            Lcd_Cmd(z);
            Lcd_Cmd(y);
            break;
        default:return;
    }
}

void Lcd_Write_Char(char a) {
    char tempp, y;
    tempp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(150);
    EN = 0;
    Lcd_Port(tempp);
    EN = 1;
    __delay_us(150);
    EN = 0;
    __delay_us(100);

    //set pull-up to filter.
    EN = 1;
    RS = 1;
    D4 = 1;
    D5 = 1;
    D6 = 1;
    D7 = 1;
}

void Lcd_Write_String(char *a) {
    unsigned char i = 0;
    //for (i = 0; a[i] != '\0' && i < 20; i++) { // anterior nao funciona a posicao 0 CGRAM
    for (i = 0; i < 20; i++) { // anterior nao funciona a posicao 0 CGRAM
        Lcd_Write_Char(a[i]);
        //        Lcd_Write_Char((a[i] != '\0') ? a[i] : ' ');
    }
}

void atualizarLCD(char *line1, char *line2, char *line3, char *line4) {

    char i = 0;
    long lcdEstado = 0;

    for (i = 0; i < 20; i++) {
        lcdEstado += (int) line1[i] + 1;
        lcdEstado += (int) line2[i] + 2;
        lcdEstado += (int) line3[i] + 3;
        lcdEstado += (int) line4[i] + 4;
    }

    if (lcdEstado != ultimoEstadoLCD) {
        ultimoEstadoLCD = lcdEstado;

        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String(line1);
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String(line2);
        Lcd_Set_Cursor(3, 0);
        Lcd_Write_String(line3);
        Lcd_Set_Cursor(4, 0);
        Lcd_Write_String(line4);
    }

    for (i = 0; i < 20; i++) {
        line1[i] = ' ';
        line2[i] = ' ';
        line3[i] = ' ';
        line4[i] = ' ';
    }
}

void criarCaracteresCGRAM() {
    const char character[6][8] = {
        0x00, 0x1F, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, // icone torre de sinal
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, // Icone sinal fraco
        0x00, 0x00, 0x00, 0x04, 0x04, 0x14, 0x14, 0x14, // icone sinal medio
        0x00, 0x01, 0x01, 0x05, 0x05, 0x15, 0x15, 0x15, // icone sinal forte,
        0x0A, 0x0A, 0x1F, 0x11, 0x11, 0x0E, 0x04, 0x04, // Icone tomada,
    };
    Lcd_Cmd(0b0100);
    Lcd_Cmd(0b0000);
    __delay_us(100);
    char i = 0, j = 0, k = 0;
    for (i = 0; i < 5; i++) {
        if (k == 1) i = 0;
        for (j = 0; j < 8; j++) {
            Lcd_Write_Char((k) ? character[i][j] : 0); // adianta um valor da CGRAM
        }
        k++;
    }
}

void blink(char *line, char start, char end) {
    if (blinkk) {
        char i = 0;
        for (i = (--start); i < end; i++) {
            line[i] = ' ';
        }
    }
}

void reiniciaLCD(){
    Lcd_Init(); // Configura TRIS LCD  e inicializa
    criarCaracteresCGRAM();
   // Lcd_Clear();
    timerAtrasoLCD = 1;
    ultimoEstadoLCD = 0;
}

#endif	

