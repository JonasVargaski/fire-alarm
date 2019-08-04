
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
                        etapaComunicacao = 0;
                        writeChar(0x1A);
                        intensidadeSinal = 'x'; // se o modulo nao responder exibe um x indicando erro no modulo.
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
                        timerVerificarSinal = 15; // verificar sinal a cada 15 segundos.
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
                timerEtapaComunicacao = 5;
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
                writeChar(0x1A);
            }
            break;
        case 2:
            if (aguardaBuffer("OK")) {
                limparBuffer();
                printf("AT+CMGS=\"+55%s\"\r\n", telefoneSelecionado == 0 ? tel1 : tel2);
                timerEtapaComunicacao = 5;
                etapaComunicacao++;
                break;
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
            }
            break;
        case 3:
            if (aguardaBuffer(">")) {
                limparBuffer();
                printf("TESTE DE BOMBAS REALIZADO\r\nEM %02d:%02d:%02d %02d/%02d/%02d\r\nJOCKEY: %s\r\nPRINCIPAL: %s\r\nCOMBUSTAO: %s\r\n",
                        _hor, _min, _sec, _dia, _mes, _ano, status_jockey == 1 ? "OK" : "FALHA", status_principal == 1 ? "OK" : "FALHA", status_estacionaria == 1 ? "OK" : "FALHA");
                delay(200);
                writeChar(0x1A);
                etapaComunicacao++;
                timerEtapaComunicacao = 30; // Tempo que fica no aguardo até enviar o sms
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
            }
            break;
        case 4:
            if (aguardaBuffer("OK")) {
                if (telefoneSelecionado == 0) {
                    telefoneSelecionado = 1;
                } else {
                    telefoneSelecionado = 0;
                    gsmOcupado = false;
                }
                etapaComunicacao = 0;
            }
            if (aguardaBuffer("ERROR")) {
                etapaComunicacao = 0;
            }
            if (!timerEtapaComunicacao) {
                etapaComunicacao = 0;
            }
            break;
        default:
            etapaComunicacao = 0;
            break;

    }
}

void enviaSMS(unsigned char tipo) { // 1 para disparo , 2 para teste de bombas, 3 para atualizar status
    switch (estagio_envio_SMS) {
        case 0:
            limparBuffer();
            printf("AT\r\n");
            if (aguardaBuffer("OK")) {
                estagio_envio_SMS++;
                status_SIM800L = OK;
            } else {
                while (!TXSTAbits.TRMT); // verifica se ainda tem dados para serem transmitidos
                TXREG = 0x1A; // Envia final de mensagen
                delay(15);
                printf("ATH0\r\n"); // desconecta chamada
                estagio_envio_SMS = 0;
                status_SIM800L = ERRO;
            }
            if (tipo == 3) { // Retorna se for somente para atualizar status do modulo
                estagio_envio_SMS = 0;
                return;
            }
            break;
        case 1:
            if (tempo_reenvio_SMS >= INTERVALO_ENVIO_SMS) { // Intervalo entre envios de sms 
                limparBuffer();
                printf("AT+CMGF=1\r\n");
                if (aguardaBuffer("OK")) {
                    estagio_envio_SMS++;
                    timer_envio_sms = 0;
                } else {
                    status_SIM800L = ERRO;
                    estagio_envio_SMS = 0;
                }
            } else {
                //timer_envio_sms = 0;
                return; // para nao enviar SMS direto.. apenas a cada 2 minutos
            }
            break;
        case 2:
            limparBuffer();
            printf("AT+CMGS=\"+55%s\"\r\n", tel_selecionado == 0 ? tel1 : tel2);
            if (aguardaBuffer(">")) {
                estagio_envio_SMS++;
            } else {
                estagio_envio_SMS = 0;
            }
            break;
        case 3:
            if (tipo == 1) {
                printf("DISPARO ALARME DE INCENDIO\r\nEM %02d:%02d:%02d %02d/%02d/%02d\r\n", _hor, _min, _sec, _dia, _mes, _ano);
            }
            if (tipo == 2) {
                printf("TESTE DE BOMBAS REALIZADO\r\nEM %02d:%02d:%02d %02d/%02d/%02d\r\nJOCKEY: %s\r\nPRINCIPAL: %s\r\nCOMBUSTAO: %s\r\n",
                        _hor, _min, _sec, _dia, _mes, _ano, status_jockey == 1 ? "OK" : "FALHA", status_principal == 1 ? "OK" : "FALHA", status_estacionaria == 1 ? "OK" : "FALHA");
            }
            estagio_envio_SMS++;
            return;
            break;
        case 4:
            while (!TXSTAbits.TRMT); // verifica se ainda tem dados para serem transmitidos
            TXREG = 0x1A; // Envia final de mensagen
            delay(10);
            while (!TXSTAbits.TRMT); // verifica se ainda tem dados para serem transmitidos
            TXREG = 0x1A; // Envia final de mensagen
            delay(10);
            timer_envio_sms = 0;
            estagio_envio_SMS++;
            break;
        case 5:
            if (timer_envio_sms > TEMPO_ENVIO_SMS) { // Aguarda 20 segundos até enviar a msg;
                estagio_envio_SMS++;
            }
            break;
        case 6:
            if (tipo == 1) { // só faz ligaçao quando for disparo de incendio
                limparBuffer();
                printf("ATD 0%s;\r\n", tel_selecionado == 0 ? tel1 : tel2);
                estagio_envio_SMS++;
                timer_envio_sms = 0;
            } else {
                estagio_envio_SMS = 8;
            }
            break;
        case 7:
            if (timer_envio_sms > TEMPO_DURACAO_CHAMADA) { //  segundos efetuando a chamada
                //printf("AT+HVOIC\r\n"); // desconecta chamada
                printf("ATH0\r\n"); // desconecta chamada
                delay(100);
                estagio_envio_SMS++;
            }
            break;
        case 8:
            tel_selecionado++;
            if (tel_selecionado > 1) {
                tel_selecionado = 0;
                tempo_reenvio_SMS = 0;
                teste_sms_run = 0;
            }
            estagio_envio_SMS = 0;
            break;
        default: estagio_envio_SMS = 0;
            break;
    }
}


#endif

