
#ifndef CONTROLE_H
#define	CONTROLE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "shift595.h"
#include "teclado.h"
#include "eeprom.h"
#include "comunicacao.h"


void acaoBombas(void);
void partidaBombaEstacionaria(unsigned char acao);
void acaoTesteBombas(void);
void verificarIntervaloTesteBombas(void);

void acaoBombas() {
    if (pressao >= sp_pressao_rede && !ocorrendoIncendio && !executandoTeste) { // Se tiver erro na leitura do transdutor desliga as bombas somente se nao tiver incendio
        shift[rl_jockey] = 0;
        shift[rl_principal] = 0;
        partidaBombaEstacionaria(0); // desliga a bomba estacionaria
        shift[rl_alarme] = 0;
        return;
    }

    shift[rl_sol_despressurizacao] = 0; // fecha solenoide ladrão
    if (pressao <= sp_jockey) { // se abaixar a pressao até o setpoint da jockey liga a jockey
        shift[rl_jockey] = 1;
    } else if (pressao >= sp_pressao_rede && !ocorrendoIncendio) {
        shift[rl_jockey] = 0;
    }

    if (pressao <= sp_principal) {
        ocorrendoIncendio = true; // sinaliza e trava que o alarme de incendio esta acionado
        shift[rl_principal] = 1;
        //        if (FALTA_ENERGIA) { // liga bomba estacionaria caso nao tenha energia no ligar da principal
        //            partidaBombaEstacionaria(1);
        //        }
    }
    if (pressao >= sp_pressao_rede && !ocorrendoIncendio) { // se pressao atingir o desejado, entao desliga todas as bombas, somente se nao tiver incendio
        shift[rl_principal] = 0;
        partidaBombaEstacionaria(0);
    }

    if (pressao <= sp_estacionaria) {
        partidaBombaEstacionaria(1); // Passar valor 1 indicando para ligar
        ocorrendoIncendio = true; // sinaliza e trava que o alarme de incendio esta acionado
    }

    if (ocorrendoIncendio) { // se tiver pegando fogo..
        gsmOcupado = true;
        enviaSMS(1); // Enviar SMS de AVISO DE DISPARO
        shift[rl_alarme] = 1; // dispara alarme
    }
}

void partidaBombaEstacionaria(unsigned char acao) { // ação da bomba : 1 para ligar  / 2 : para desligar
    if (acao == 1 && !flagEstacionariaLigada) {
        if (_sec_partida != _sec) {
            _sec_partida = _sec;
            tempo_limite_partida++;
            tempo_partida++;
            if (SINAL_ESTAC_LIGADO) { // Verifica se a bomba a combustao ligou
                delay(ATRASO_ARRANQUE_COMBUSTAO); // atraso para manter o motor de arranque um poco mais tempo ligado
                flagEstacionariaLigada = true;
                tempo_partida = 0; // zera tempo para caso nao ligar, começe a contar novamente.
                shift[rl_motor_arranque] = 0;
            }
        }
    }
    if (!flagEstacionariaLigada && status_estacionaria == OK) { // LIGAR, verifica o status, caso ja tenha estourado o tempo de partida nao tenta ligar denovo
        shift[rl_sol_diesel] = 1;
        if (tempo_partida <= 7) { //7 segundo tempo ligando
            shift[rl_motor_arranque] = 1;
        } else if (tempo_partida <= (7 + 8)) { //8 segundo tempo aguardando
            shift[rl_motor_arranque] = 0;
        } else {
            tempo_partida = 0;
        }
    }
    if (tempo_limite_partida > tempo_limite_estacionaria) { // verifica se o tmepo tentando ligar é maior que o tempo limite ajustado.
        tempo_limite_partida = 0;
        status_estacionaria = ERRO;
        flagEstacionariaLigada = false;
        shift[rl_sol_diesel] = 0;
        shift[rl_motor_arranque] = 0;
    }

    ////////////////////////////////////////
    if (acao == 0) {
        flagEstacionariaLigada = false;
        shift[rl_motor_arranque] = 0;
        shift[rl_sol_diesel] = 0;
        tempo_partida = 0;
        tempo_limite_partida = 0;
    }
    ////////////////////////////////////////
    if (status_estacionaria == ERRO) {
        shift[rl_sol_diesel] = 0;
        shift[rl_motor_arranque] = 0;
        tempo_partida = 0;
    }
    if (flagEstacionariaLigada) { // se a bomba estiver ligada
        if (SINAL_ESTAC_LIGADO) { // permanece verificando se a mesma nao desligou
            shift[rl_motor_arranque] = 0;
            tempo_limite_partida = 0;
        } else {
            flagEstacionariaLigada = false; // Se a  bomba nao ligou, ou desligou no periodo, tentar ligar novamente, até estourar o tempo maximo de partida
        }
    }
}

void verificarIntervaloTesteBombas() {
    if (dias_restante_teste >= intervalo_teste_dia && habilitaTesteAutomatico == 1 && !executandoTeste) { // Se ajustado intervalo como 0 entao desativa a funçao de teste automatico
        if (_hor == horario_de_teste) {
            dias_restante_teste = 0;
            writeEEPROM_ext(10, dias_restante_teste);
            etapaTesteBombas = 0;
            executandoTeste = true;
            menu_posi = 28;
        }
    }
}

void drenarPressao() {
    sprintf(line4, "Despressurizando...");
    if (!timerTesteBombas) {
        executandoTeste = false;
        codigoErro = 3;
        menu_posi = 0;
    }
}

void acaoTesteBombas() {
    if (btPress(b_esc)) {
        executandoTeste = false;
        gsmOcupado = false;
        etapaTesteBombas = 0;
        unsigned char intervalo = readEEPROM_ext(6);
        intervalo_teste_dia = intervalo > 0 ? intervalo : 1;
        menu_posi = _menu_posi;
        char i = 0;
        for (i = 0; i < 8; i++) {
            shift[i] = 0; // desliga todos os relés
        }
    }

    if (etapaTesteBombas < 10) {
        sprintf(&line1[3], "Aguarde... %ds", timerTesteBombas);
        sprintf(&line2[2], "Pressao %s BAR", intToFloatStr(pressao));
    }

    switch (etapaTesteBombas) {
        case 0:
            dias_restante_teste = 0; // ZERA CONTADOR DE DIAS POR TESTE AUTOMATICO
            writeEEPROM_ext(10, dias_restante_teste);
            shift[rl_sol_despressurizacao] = 1;
            timerTesteBombas = 30;
            etapaTesteBombas = 1;
            break;
        case 1:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) { // - 1BAR abaixo do setpoint do sistema
                shift[rl_sol_despressurizacao] = 0;
                shift[rl_jockey] = 1;
                etapaTesteBombas = 2;
                timerTesteBombas = 50;
            }
            break;
        case 2:
            sprintf(line4, "Bomba Jockey...");
            if (pressao >= sp_pressao_rede) {
                shift[rl_jockey] = 0;
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
            shift[rl_sol_despressurizacao] = 1;
            etapaTesteBombas = 4;
            break;
        case 4:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) {
                shift[rl_principal] = 1;
                shift[rl_sol_despressurizacao] = 0;
                etapaTesteBombas = 5;
                timerTesteBombas = 50;
            }
            break;
        case 5:
            sprintf(line4, "Bomba Principal...");
            if (pressao >= sp_pressao_rede) {
                shift[rl_principal] = 0;
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
            shift[rl_sol_despressurizacao] = 1;
            etapaTesteBombas = 7;
            break;
        case 7:
            drenarPressao();
            if (pressao <= (sp_pressao_rede - 10)) {
                shift[rl_sol_despressurizacao] = 0;
                etapaTesteBombas = 8;
                timerTesteBombas = tempo_limite_estacionaria; // pega tempos das configuraçoes;
            }
            break;
        case 8:
            sprintf(line4, "Bomba Combustao...");
            partidaBombaEstacionaria(1); // Liga a bomba a combustao

            if (flagEstacionariaLigada || pressao >= sp_pressao_rede) {
                status_estacionaria = OK;
                timerTesteBombas = tempo_limite_teste_estacionaria;
                etapaTesteBombas = 9;
            }
            if (status_estacionaria == ERRO || !timerTesteBombas) {
                etapaTesteBombas = 10;
                status_estacionaria = ERRO;
            }
            break;
        case 9:
            sprintf(line4, "Bomba Ligada...");
            partidaBombaEstacionaria(1); // Liga a bomba a combustao
            if (!timerTesteBombas) {
                etapaTesteBombas = 10;
            }
            break;
        case 10:
            salvarLOG();
            partidaBombaEstacionaria(2);
            clearShiftREG();
            etapaTesteBombas = 11;
            timerTesteBombas = 10;
            executandoTeste = false;
            break;
        case 11:
            sprintf(&line1[2], "Teste Concluido!");
            sprintf(line2, "JOKEY : %s", status_jockey == 1 ? "OK" : "ERRO");
            sprintf(line3, "PRINCIPAL : %s", status_principal == 1 ? "OK" : "ERRO");
            sprintf(line4, "COMBUSTAO : %s", status_estacionaria == 1 ? "OK" : "ERRO");
            if (!timerTesteBombas) {
                etapaTesteBombas = 12;
                timerTesteBombas = 150;
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
            break;
    }
}

#endif

