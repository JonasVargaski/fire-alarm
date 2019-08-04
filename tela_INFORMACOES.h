#ifndef TELA_INFORMACOES_H
#define	TELA_INFORMACOES_H

#include <xc.h> 
#include "variaveisGlobais.h"

void informacoes() {
    sprintf(line1, "TECHNOW");
    sprintf(line2, "Sistemas embarcados");
    sprintf(line3, "   48 99906-5651");
    sprintf(line4, "versao - %s", __DATE__);
    if (btPress(b_esc)) {
        menu_posi = _menu_posi;
    }
}


#endif	