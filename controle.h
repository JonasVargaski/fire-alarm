
#ifndef CONTROLE_H
#define	CONTROLE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "variaveisGlobais.h"
#include "teclado.h"
#include "eeprom.h"
#include "comunicacao.h"

void monitorarAlteracaoEstado(void);
void acaoBombas(void);
void partidaBombaEstacionaria(unsigned char acao);
void acaoTesteBombas(void);
void verificarIntervaloTesteBombas(void);

void acaoBombas() {
    if (pressao < sp_jockey && timerIntervaloEntreBombas == 0 && !out_RL_JOCKEY) { // se abaixar a pressao atï¿½ o setpoint da jockey liga a jockey
        timerIntervaloEntreBombas = 4; // após abaixar pressão, leva 4 segundos para partir a proxima bomba
        out_RL_JOCKEY = 1;
    } else if (pressao >= sp_pressao_rede && ocorrendoIncendio == 0) {
        out_RL_JOCKEY = 0;
    }

    if (pressao <= sp_estacionaria && timerIntervaloEntreBombas == 0 && !out_BOMBA_ESTACIONARIA) {
        ocorrendoIncendio = 1; // sinaliza e trava que o alarme de incendio esta acionado
        timerIntervaloEntreBombas = 3; // após abaixar pressão, leva 3 segundos para partir a proxima bomba
        out_BOMBA_ESTACIONARIA = 1; // Passar valor 1 indicando para ligar
    }

    if (pressao <= sp_principal && timerIntervaloEntreBombas == 0 && !out_RL_PRINCIPAL) {
        timerIntervaloEntreBombas = 4; // após abaixar pressão, leva 4 segundos para partir a proxima bomba
        ocorrendoIncendio = 1; // sinaliza e trava que o alarme de incendio esta acionado
        out_RL_PRINCIPAL = 1;
    }

    if (ocorrendoIncendio == 1) { // se tiver pegando fogo..
        gsmOcupado = true;
        enviaSMS(1); // Enviar SMS de AVISO DE DISPARO
    }
}

void partidaBombaEstacionaria(unsigned char acao) { // aï¿½ï¿½o da bomba : 1 para ligar  / 2 : para desligar
    if (acao && status_estacionaria == OK) {
        out_RL_SOLENOIDE1 = 1; //abre solenoide do diesel para bomba

        if (SINAL_ESTAC_LIGADO) { // Verifica se a bomba a combustao ligou
            tempo_partida = 11; // aguarda um pequeno intervalo para tentar religar
            out_RL_ESTACIONARIA = 0;
        } else {
            if (tempo_partida <= 7) { //7 segundo tempo do motor de arranque ligando
                out_RL_ESTACIONARIA = 1;
            } else if (tempo_partida <= (7 + 8)) { //8 segundo tempo aguardando
                out_RL_ESTACIONARIA = 0;
            } else {
                tempo_partida = 0; // começa ciclo novamentew
            }
        }

        if (tempo_limite_partida > tempo_limite_estacionaria) { // verifica se o tmepo tentando ligar ï¿½ maior que o tempo limite ajustado.
            tempo_limite_partida = 0;
            status_estacionaria = ERRO;
        }

    } else {
        out_RL_ESTACIONARIA = 0;
        out_RL_SOLENOIDE1 = 0;
        tempo_partida = 0;
        tempo_limite_partida = 0;
    }
}

void verificarIntervaloTesteBombas() {
    if (intervalo_teste_dia != 0 && dias_restante_teste >= intervalo_teste_dia && habilitaTesteAutomatico == 1) { // Se ajustado intervalo como 0 entao desativa a funï¿½ao de teste automatico
        if (_hor == horario_de_teste) {
            dias_restante_teste = 0;
            writeEEPROM_ext(10, dias_restante_teste);
            etapaTesteBombas = 0;
            menu_posi = 28;
        }
    }
}

void drenarPressao() {
    sprintf(line4, "Despressurizando...");
    if (!timerTesteBombas) {
        codigoErro = 3;
        menu_posi = 0;
    }
}

void monitorarAlteracaoEstado(void) {
    int estado = 0;

    if (out_RL_ESTACIONARIA) estado += 1;
    if (out_RL_JOCKEY) estado += 2;
    if (out_RL_ALARME) estado += 3;
    if (out_RL_PRINCIPAL) estado += 4;
    if (out_RL_SOLENOIDE1) estado += 5;
    if (out_RL_SOLENOIDE2) estado += 6;

    if (estado != ultimoEstadoSaidas) {
        ultimoEstadoSaidas = estado;
        timerAtrasoLCD = 13; // 1,3 segundos para voltar a atualizar o lcd
    }
}

void acaoTesteBombas() {
    if (btPress(b_esc)) {
        gsmOcupado = false;
        etapaTesteBombas = 0;
        out_BOMBA_ESTACIONARIA = 0;
        menu_posi = _menu_posi;
        char i = 0;
    }

    if (etapaTesteBombas < 10) {
        sprintf(&line1[3], "Aguarde... %ds", timerTesteBombas);
        sprintf(&line2[2], "Pressao %s BAR", intToFloatStr(pressao));
    }

    switch (etapaTesteBombas) {
        case 0:
            dias_restante_teste = 0; // ZERA CONTADOR DE DIAS POR TESTE AUTOMATICO
            writeEEPROM_ext(10, dias_restante_teste);
            out_RL_SOLENOIDE2 = 1;
            timerTesteBombas = 30;
            etapaTesteBombas = 1;
            break;
        case 1:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) { // - 1BAR abaixo do setpoint do sistema
                out_RL_SOLENOIDE2 = 0;
                out_RL_JOCKEY = 1;
                etapaTesteBombas = 2;
                timerTesteBombas = 50;
            }
            break;
        case 2:
            sprintf(line4, "Bomba Jockey...");
            if (pressao >= sp_pressao_rede) {
                out_RL_JOCKEY = 0;
                status_jockey = OK;
                etapaTesteBombas = 3;
            }
            if (!timerTesteBombas) {
                status_jockey = ERRO;
                etapaTesteBombas = 3;
                timerTesteBombas = 30;
            }
            break;
        case 3:
            out_RL_SOLENOIDE2 = 1;
            etapaTesteBombas = 4;
            break;
        case 4:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) {
                out_RL_PRINCIPAL = 1;
                out_RL_SOLENOIDE2 = 0;
                etapaTesteBombas = 5;
                timerTesteBombas = 50;
            }
            break;
        case 5:
            sprintf(line4, "Bomba Principal...");
            if (pressao >= sp_pressao_rede) {
                out_RL_PRINCIPAL = 0;
                status_principal = OK;
                etapaTesteBombas = 6;
            }
            if (!timerTesteBombas) {
                status_principal = ERRO;
                etapaTesteBombas = 6;
                timerTesteBombas = 30;
            }
            break;
        case 6:
            out_RL_SOLENOIDE2 = 1;
            etapaTesteBombas = 7;
            break;
        case 7:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) {
                out_RL_SOLENOIDE2 = 0;
                out_BOMBA_ESTACIONARIA = 1;
                etapaTesteBombas = 8;
                timerTesteBombas = tempo_limite_estacionaria; // pega tempos das configuraï¿½oes;
            }
            break;
        case 8:
            sprintf(line4, "Bomba Combustao...");
            if (SINAL_ESTAC_LIGADO || pressao >= sp_pressao_rede) {
                status_estacionaria = OK;
                timerTesteBombas = tempo_limite_teste_estacionaria;
                etapaTesteBombas = 9;
            }
            if (status_estacionaria == ERRO || !timerTesteBombas) {
                etapaTesteBombas = 10;
                status_estacionaria = ERRO;
                out_BOMBA_ESTACIONARIA = 0;
            }
            break;
        case 9:
            sprintf(line4, "Bomba %s", SINAL_ESTAC_LIGADO ? "Ligada..." : "Religando...");
            if (!timerTesteBombas || status_estacionaria == ERRO) {
                out_RL_ESTACIONARIA = 0;
                etapaTesteBombas = 10;
            }
            break;
        case 10:
            salvarLOG();
            out_RL_ESTACIONARIA = 0;
            etapaTesteBombas = 11;
            timerTesteBombas = 10;
            break;
        case 11:
            sprintf(&line1[2], "Teste Concluido!");
            sprintf(line2, "JOKEY : %s", status_jockey == 1 ? "OK" : "ERRO");
            sprintf(line3, "PRINCIPAL : %s", status_principal == 1 ? "OK" : "ERRO");
            sprintf(line4, "COMBUSTAO : %s", status_estacionaria == 1 ? "OK" : "ERRO");
            out_BOMBA_ESTACIONARIA = 0;
            if (!timerTesteBombas) {
                etapaTesteBombas = 12;
                timerTesteBombas = 80;
                timerReenvioSMS = 0;
                gsmOcupado = true;
            }
            break;
        case 12:
            sprintf(&line4[2], "enviando SMS...");
            enviaSMS(2); // somente sms // tipo teste de bombas
            if (!timerTesteBombas) {
                menu_posi = 0;
                gsmOcupado = false;
            }
            break;
        default:
            etapaTesteBombas = 0;
            menu_posi = 0;
            out_BOMBA_ESTACIONARIA = 0;
            break;
    }
}

#endif

