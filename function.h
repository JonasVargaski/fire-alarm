#ifndef FUNCOES_H
#define	FUNCOES_H

#include "config.h"
#include <xc.h>
#include <stdio.h>

void delay(int x);
char DECtoBCD(char val);
char BCDtoDEC(char val);
int DECtoHEX(int hex);
char* intToFloatStr(unsigned char valor);

#endif
