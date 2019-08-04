 
#ifndef TELA_TELEFONES_H
#define	TELA_TELEFONES_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"

void configuraTelefones() {
    sprintf(line1, " T1: %s", tel1);
    sprintf(line2, " T2: %s", tel2);
    sprintf(line3, " %s", teste_sms_run == 1 ? "ENVIANDO SMS... " : "ENVIAR SMS TESTE");
    sprintf(line4, "  GSM: %s", status_SIM800L == 1 ? "OK  " : "ERRO");

    if (ajust_tel == 0) { // se nao tiver sendo ajustado a hora
        if (btPress(b_mais)) {
            option_posi--;
        }
        if (btPress(b_menos)) {
            option_posi++;
        }
    }
    if (ajust_tel == 1) {
        if (btPress(b_ok)) {
            tel_posi++;
        }
    }
    if (btPress(b_esc)) {
        teste_sms_run = 0;
        tempo_reenvio_SMS = 5; // reseta tempo para reenvio de sms
        option_posi = 0;
        tel_posi = 0;
        ajust_tel = 0;
        char i = 0;
        for (i = 0; i < 11; i++) {
            writeEEPROM_ext((20 + i), tel1[i]);
            writeEEPROM_ext((40 + i), tel2[i]);
        }
        menu_posi = _menu_posi;
    }

    switch (option_posi) {
        case 0:
            //            enviaSMS(3); // Checa Status do modulo;
            option_posi = 1;
            break;

        case 1:
            line1[0] = '>';
            if (btPress(b_ok)) {
                ajust_tel = 1;
            }
            if (ajust_tel) {
                line1[(tel_posi + 4)] = '>';
                if ((tel_posi + 2) > sizeof (tel1)) { // PARA nao estorar o array qe tem o telefone
                    tel_posi = 0;
                    ajust_tel = 0;
                }
                if (btPress(b_mais)) {
                    tel1[tel_posi]++;
                }
                if (btPress(b_menos)) {
                    tel1[tel_posi]--;
                }
                if ((tel1[tel_posi] > 57) | (tel1[tel_posi] < 48)) { // > 9 tabela ascii
                    tel1[tel_posi] = 48; // = 0 tabela ascii
                }
            }
            break;
        case 2:
            line2[0] = '>';
            if (btPress(b_ok)) {
                ajust_tel = 1;
            }
            if (ajust_tel) {
                line2[(tel_posi + 4)] = '>';
                if ((tel_posi + 2) > sizeof (tel2)) { // PARA nao estorar o array qe tem o telefone
                    tel_posi = 0;
                    ajust_tel = 0;
                }
                if (btPress(b_mais)) {
                    tel2[tel_posi]++;
                }
                if (btPress(b_menos)) {
                    tel2[tel_posi]--;
                }
                if ((tel2[tel_posi] > 57) | (tel2[tel_posi] < 48)) { // > 9 tabela ascii
                    tel2[tel_posi] = 48; // = 0 tabela ascii
                }
            }
            break;
        case 3:
            line3[0] = '>';
            if (btPress(b_ok)) {
                teste_sms_run = 1;
                tel_selecionado = 0;
                tempo_reenvio_SMS = 6; // Para desconsiderar o tempo, e enviare quando apertar
            }
            break;

        default:option_posi = 0;
            break;
    }
    if (teste_sms_run == 1 && status_SIM800L == OK) { // Se apertar pra testar vai enviar sms
        //        enviaSMS(2);
    } else {
        teste_sms_run = 0;
    }

}



#endif	


