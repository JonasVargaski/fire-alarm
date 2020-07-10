
#ifndef TELA_TELEFONES_H
#define	TELA_TELEFONES_H

#include <xc.h> 
#include "variaveisGlobais.h"
#include "teclado.h"

void configuraTelefones() {
    sprintf(&line1[1], "T1: %s", tel1);
    sprintf(&line2[1], "T2: %s", tel2);
    sprintf(&line3[1], "%s", gsmOcupado ? " aguarde... " : "Enviar sms teste");
    //#CONTROLE
    if (ajust_tel == 0) { // se nao tiver sendo alterado o telefone
        if (btPress(b_mais)) {
            option_posi--;
        } else if (btPress(b_menos)) {
            option_posi++;
        } else if (btPress(b_esc)) {
            gsmOcupado = false;
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
    }
    if (ajust_tel == 1) {
        if (btPress(b_ok)) {
            tel_posi++;
        } else if (btPress(b_esc)) {
            tel_posi = 0;
            ajust_tel = 0;
        }
    }

    switch (option_posi) {
        case 0:
            line1[0] = '>';
            blink(line1,1,1);
            if (btPress(b_ok)) {
                ajust_tel = 1;
            }
            if (ajust_tel) {
                blink(line1, (tel_posi + 6), (tel_posi + 6));
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
        case 1:
            line2[0] = '>';
            blink(line2,1,1);
            if (btPress(b_ok)) {
                ajust_tel = 1;
            }
            if (ajust_tel) {
                blink(line2, (tel_posi + 6), (tel_posi + 6));
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
        case 2:
            line3[0] = '>';
            blink(line3,1,1);
            if (btPress(b_ok) && intensidadeSinal != 'x' && !gsmOcupado) {
                gsmOcupado = true;
                telefoneSelecionado = 0;
            }
            break;

        default:option_posi = 0;
            break;
    }
    if (gsmOcupado) { // ENVIAR MENSAGENS DE TEXTO PARA TESTE DO MODULO
        comunicarTeste();
    }

}



#endif	


