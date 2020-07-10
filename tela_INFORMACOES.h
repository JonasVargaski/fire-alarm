#ifndef TELA_INFORMACOES_H
#define	TELA_INFORMACOES_H

#include <xc.h> 
#include "variaveisGlobais.h"

void informacoes() {
    sprintf(line1, "TECHNOW");
    sprintf(line2, "Sistemas embarcados");
    sprintf(line3, "%d-%d-%d-%d-%d-%d > %d-%d", out_RL_JOCKEY, out_RL_PRINCIPAL, out_RL_ESTACIONARIA, out_RL_SOLENOIDE1, out_RL_SOLENOIDE2, out_RL_ALARME, SINAL_ESTAC_LIGADO, SINAL_AUX2);
    sprintf(line4, "versao - %s", __DATE__);
    //#CONTROLE
    if (btPress(b_esc)) {
        menu_posi = _menu_posi;
    }
}


#endif	