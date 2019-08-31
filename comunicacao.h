
#ifndef COMUNICACAO_H
#define	COMUNICACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "config.h"
#include "variaveisGlobais.h"
#include "serial.h"
#include "ds1307.h"

void getSinalSIM800L() {
    if (!timerVerificarSinal) {
        if (!gsmOcupado) {
            switch (etapaComunicacao) {
                case 0:
                    limparBuffer();
                    printf("AT+CSQ\r\n");
                    timerEtapaComunicacao = 10; // 10 segundos para esperar pela resposta do modulo gsm
                    etapaComunicacao = 1;
                    break;
                case 1:
                    if (!timerEtapaComunicacao) {
                        printf("AT\r\n");
                        delay(100);
                        if (aguardaBuffer("OK")) {
                            etapaComunicacao = 0;
                        } else {
                            etapaComunicacao = 0;
                            writeChar(0x1A);
                            intensidadeSinal = 'x'; // se o modulo nao responder exibe um x indicando erro no modulo.
                        }
                    }
                    if (aguardaBuffer("+CSQ: ")) {
                        int sinal = atoi(getSerial("+CSQ: ", ','));
                        if (sinal < 10) {
                            intensidadeSinal = ' ';
                        } else if (sinal >= 10 && sinal <= 14) {
                            intensidadeSinal = ICONE_SINAL_FRACO;
                        } else if (sinal >= 16 && sinal <= 19) {
                            intensidadeSinal = ICONE_SINAL_MEDIO;
                        } else if (sinal >= 20 && sinal <= 30) {
                            intensidadeSinal = ICONE_SINAL_BOM;
                        }
                        etapaComunicacao = 0;
                        timerVerificarSinal = 6; // verificar sinal a cada 6 segundos.
                    }
                    break;
                default:
                    etapaComunicacao = 0;
            }
        }
    }
}

void comunicarTeste() {
    switch (etapaComunicacao) {
        case 0:
            limparBuffer();
            printf("AT\r\n");
            etapaComunicacao++;
            timerEtapaComunicacao = 5;
            break;
        case 1:
            if (aguardaBuffer("OK")) {
                limparBuffer();
                printf("AT+CMGF=1\r\n");
                etapaComunicacao++;
                timerEtapaComunicacao = 2;
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
                writeChar(0x1A);
            }
            break;
        case 2:
            if (!timerEtapaComunicacao) {
                limparBuffer();
                printf("AT+CMGS=\"+55%s\"\r\n", telefoneSelecionado == 0 ? tel1 : tel2);
                timerEtapaComunicacao = 2;
                etapaComunicacao++;
            }
            break;
        case 3:
            if (!timerEtapaComunicacao) {
                limparBuffer();
                printf("TECHNOW Sistemas embarcados\r\n\r\nTeste manual de comunicacao.\r\nData: %02d/%02d/%02d %02d:%02d:%02d\r\n\r\nVersao do sistema: %s\r",
                        _dia, _mes, _ano, _hor, _min, _sec, __DATE__);
                               writeChar(0x1A);
                delay(100);
                writeChar(0x1A);
                delay(100);
                writeChar(0x1A);
                etapaComunicacao++;
                timerEtapaComunicacao = 30; // Tempo que fica no aguardo até enviar o sms
            }
            break;
        case 4:
            if (aguardaBuffer("OK") || aguardaBuffer("+CMGS:")) {
                if (telefoneSelecionado == 0) {
                    telefoneSelecionado = 1;
                } else {
                    telefoneSelecionado = 0;
                    gsmOcupado = false;
                }
                etapaComunicacao = 0;
            }
            if (aguardaBuffer("ERROR") || !timerEtapaComunicacao) {
                etapaComunicacao = 0;
            }
            break;
        default:
            etapaComunicacao = 0;
            break;

    }
}

void enviaSMS(char tipo) { // 1 para disparo , 2 para teste de bombas
    switch (etapaComunicacao) {
        case 0:
            limparBuffer();
            printf("AT\r\n");
            etapaComunicacao++;
            timerEtapaComunicacao = 6;
            break;
        case 1:
            if (aguardaBuffer("OK")) {
                limparBuffer();
                printf("AT+CMGF=1\r\n");
                etapaComunicacao++;
                timerEtapaComunicacao = 2;
            }
            if (!timerEtapaComunicacao) {
                writeChar(0x1A);
                delay(15);
                writeChar(0x1A);
                printf("ATH0\r\n"); // desconecta chamada
                etapaComunicacao = 0;
            }
            break;
        case 2:
            if (!timerEtapaComunicacao) {
                limparBuffer();
                printf("AT+CMGS=\"+55%s\"\r\n", telefoneSelecionado == 0 ? tel1 : tel2);
                timerEtapaComunicacao = 2;
                etapaComunicacao++;
            }
            break;

        case 3:
            if (!timerEtapaComunicacao) {
                limparBuffer();
                if (tipo == 1) {
                    printf("DISPARO ALARME DE INCENDIO\r\nEM %02d:%02d:%02d %02d/%02d/%02d\r\n", _hor, _min, _sec, _dia, _mes, _ano);
                }
                if (tipo == 2) {
                    printf("TESTE DE BOMBAS REALIZADO\r\nEM %02d:%02d:%02d %02d/%02d/%02d\r\nJOCKEY: %s\r\nPRINCIPAL: %s\r\nCOMBUSTAO: %s\r\n",
                            _hor, _min, _sec, _dia, _mes, _ano, status_jockey == 1 ? "OK" : "FALHA", status_principal == 1 ? "OK" : "FALHA", status_estacionaria == 1 ? "OK" : "FALHA");
                }
                writeChar(0x1A);
                delay(100);
                writeChar(0x1A);
                delay(100);
                writeChar(0x1A);
                timerEtapaComunicacao = 30;
                etapaComunicacao++;
            }
            break;
        case 4:
            if (aguardaBuffer("OK") || aguardaBuffer("ERROR") || aguardaBuffer("+CMGS:")) {
                if (tipo == 2) {
                    if (telefoneSelecionado == 0) {
                        telefoneSelecionado = 1;
                        etapaComunicacao = 0;
                        return;
                    } else {
                        telefoneSelecionado = 0;
                        gsmOcupado = false;
                        menu_posi = 0; // retorna ao menu inicial, somente em testes
                    }
                } else {
                    etapaComunicacao++;
                }
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
            }
            break;
        case 5:
            limparBuffer();
            printf("ATD 0%s;\r\n", telefoneSelecionado == 0 ? tel1 : tel2);
            timerEtapaComunicacao = TEMPO_DURACAO_CHAMADA;
            etapaComunicacao++;
            break;
        case 6:
            if (!timerEtapaComunicacao || aguardaBuffer("NO")) {
                limparBuffer();
                printf("ATH0\r\n"); // desconecta chamada
                delay(550);
                if (telefoneSelecionado == 0) {
                    telefoneSelecionado = 1;
                } else {
                    telefoneSelecionado = 0;
                    gsmOcupado = false;
                    timerReenvioSMS = INTERVALO_ENVIO_SMS;
                }
                etapaComunicacao = 0;
            }
            break;

        default: etapaComunicacao = 0;
            break;
    }
}


#endif

