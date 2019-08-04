#ifndef TEMPORIZADOR_H
#define	TEMPORIZADOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "ds1307.h"
#include "eeprom.h"

void timer() {

    if (ajst_rtc) { // Se tiver sendo ajustado entao ele nao soma as variavies
        return;
    }
    timer_envio_sms++;
    if (timer_envio_sms > 100) {
        timer_envio_sms = 100;
    }
    _sec++;
    if (_sec > 59) {
        _sec = 0;
        _min++;
        if (_min > 59) {
            _min = 0;
            _hor++;
            if (_hor > 23) {
                _hor = 0;
                dias_restante_teste++;
                writeEEPROM_ext(10, dias_restante_teste);
            }
        }
    }

    if (rtc.min != _min) { // Se o minuto for diferente que estiver no rtc, entao sincroniza com timer do pic; // SERVE PARA REALIZAR 1 LEITURA POR MINUTO NO DS1307

        if (estacionaria_Ligada) {
            horimetro++;
            writeEEPROM16_ext(8, horimetro);
        }
        tempo_reenvio_SMS++;
        if (tempo_reenvio_SMS >= 7) {
            tempo_reenvio_SMS = 6; // Libera para enviar sms novamente
        }

        getDS1307TimeDate(&rtc); // busca horario no rtc;
        if ((rtc.sec > 60) && (rtc.min > 60) && (rtc.hour > 24)) { // Confirma que o rtc nao est� em falha, ou parado;
            // SE DER PROBLEMA NO MODULO RTC, ENTAO NAO COLOCA NUMEROS ESTRANHOS. Continua Somente o timer do pic e acusa falha no modulo relogio(Implemetar falha);
            falha = 2; /// falha no rel�gio.
            RTC_Init(); // for�a o reinicio do DS1307 ci
            rtc.sec = _sec; // devolve as variavies o timer do pic
            rtc.min = _min;
            rtc.hour = _hor;
            rtc.date = _dia;
            rtc.month = _mes;
            rtc.year = _ano;
            return;
        }
        _sec = rtc.sec;
        _min = rtc.min;
        _hor = rtc.hour;
        _dia = rtc.date;
        _mes = rtc.month;
        _ano = rtc.year;
    }
}

#endif	