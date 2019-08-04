
#ifndef COMUNICACAO_H
#define	COMUNICACAO_H

#include "config.h"
#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include <stdio.h>

void getSinal() {
    limparBuffer();
    printf("AT+CSQ\r\n");
    if (aguardaBuffer("+CSQ: ")) {
        int sinal = atoi(getSerial("+CSQ: ", ','));
        if (sinal >= 10 && sinal <= 14) {
            return ICONE_SINAL_FRACO;
        } else if (sinal >= 16 && sinal <= 19) {
            return ICONE_SINAL_MEDIO;
        } else if (sinal >= 20 && sinal <= 30) {
            return ICONE_SINAL_BOM;
        }
        sprintf(line3, "Sinal: %d", sinal);
    }
}

void enviaSMS(unsigned char tipo) { // 1 para disparo , 2 para teste de bombas, 3 para atualizar status

    switch (estagio_envio_SMS) {
        case 0:
            printf("AT\r\n");
            if (getRespostaUART("OK", 400)) {
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
                printf("AT+CMGF=1\r\n");
                if (getRespostaUART("OK", 600)) {
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
            printf("AT+CMGS=\"+55%s\"\r\n", tel_selecionado == 0 ? tel1 : tel2);
            if (getRespostaUART(">", 800)) {
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

