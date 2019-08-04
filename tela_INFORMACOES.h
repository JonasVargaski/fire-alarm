
#ifndef TELA_INFORMACOES_H
#define	TELA_INFORMACOES_H

#include <xc.h> 


void informacoes() {
    sprintf(line1, "  CONTRA FOGO JUNG");
    sprintf(line2, "    48 3466-2875");
    //sprintf(line3, "   48 99906-5651");
    sprintf(line4, "versao - %s", __DATE__);
    if (btPress(b_esc)) {
        menu_posi = _menu_posi;
    }
}


#endif	

