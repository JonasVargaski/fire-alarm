
#ifndef CONTROLE_H
#define	CONTROLE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "variaveisGlobais.h"
#include "shift595.h"
#include "teclado.h"


void acaoBombas(void);
void partidaBombaEstacionaria(unsigned char acao);
void acaoTesteBombas(void);
void acaoTesteBomasProgramado(void);

void acaoBombas() {
    if (pressao >= sp_pressao_rede && incendio != 1 && teste_run == 0) { // Se tiver erro na leitura do transdutor desliga as bombas somente se nao tiver incendio
        shift[rl_jockey] = 0;
        shift[rl_principal] = 0;
        partidaBombaEstacionaria(0); // desliga a bomba estacionaria
        shift[rl_alarme] = 0;
        return;
    }
    if (teste_run == 0) { // se nao tiver em teste das bombas executa.
        shift[rl_sol_despressurizacao] = 0; // Garante que a solenoide de despressurizaçao fique fechada
        if (pressao <= sp_jockey) { // se abaixar a pressao até o setpoint da jockey liga a jockey
            shift[rl_jockey] = 1;
        }
        if (pressao >= sp_pressao_rede) { // se abaixar a pressao até o setpoint da jockey liga a jockey
            shift[rl_jockey] = 0;
        }
        if (pressao >= sp_pressao_rede && incendio == 0) { // se pressao atingir o desejado, entao desliga todas as bombas, somente se nao tiver incendio
            shift[rl_principal] = 0;
            partidaBombaEstacionaria(0);
        }
        if (pressao <= sp_principal) {
            incendio = 1; // sinaliza e trava que o alarme de incendio esta acionado
            shift[rl_principal] = 1;
        }
        if (pressao <= sp_estacionaria) {
            incendio = 1;
            partidaBombaEstacionaria(1); // Passar valor 1 indicando para ligar
        }
        if (incendio) { // se tiver pegando fogo..
            //            enviaSMS(1); // Enviar SMS de AVISO DE DISPARO
            shift[rl_alarme] = 1;
        } else {
            estagio_envio_SMS = 0;
            tempo_reenvio_SMS = INTERVALO_ENVIO_SMS;
        }
    }
}

void partidaBombaEstacionaria(unsigned char acao) { // ação da bomba : 1 para ligar  / 2 : para desligar
    if (acao == 1 && estacionaria_Ligada == 0) {
        if (_sec_partida != _sec) {
            _sec_partida = _sec;
            tempo_limite_partida++;
            tempo_partida++;
            if (SINAL_ESTAC_LIGADO) { // Verifica se a bomba a combustao ligou
                delay(ATRASO_ARRANQUE_COMBUSTAO); // atraso para manter o motor de arranque um poco mais tempo ligado
                estacionaria_Ligada = 1;
                tempo_partida = 0; // zera tempo para caso nao ligar, começe a contar novamente.
                shift[rl_motor_arranque] = 0;
            }
        }
    }
    if (estacionaria_Ligada == 0 && status_estacionaria == OK) { // LIGAR, verifica o status, caso ja tenha estourado o tempo de partida nao tenta ligar denovo
        shift[rl_sol_diesel] = 1;
        if (tempo_partida <= 7) { //7 segundo tempo ligando
            shift[rl_motor_arranque] = 1;
        } else if (tempo_partida <= (7 + 5)) { //5 segundo tempo aguardando
            shift[rl_motor_arranque] = 0;
        } else {
            tempo_partida = 0;
        }
    }
    if (tempo_limite_partida > tempo_limite_estacionaria) { // verifica se o tmepo tentando ligar é maior que o tempo limite ajustado.
        tempo_limite_partida = 0;
        falha = 1; // erro na partida por tempo limite.
        status_estacionaria = ERRO;
        estacionaria_Ligada = 0;
        shift[rl_sol_diesel] = 0;
        shift[rl_motor_arranque] = 0;
    }

    ////////////////////////////////////////
    if (acao == 0) {
        estacionaria_Ligada = 0;
        shift[rl_motor_arranque] = 0;
        shift[rl_sol_diesel] = 0;
        tempo_partida = 0;
        tempo_limite_partida = 0;
    }
    ////////////////////////////////////////
    if (status_estacionaria == ERRO) {
        shift[rl_sol_diesel] = 0;
        shift[rl_motor_arranque] = 0;
        falha = 1;
        tempo_partida = 0;
    }
    if (estacionaria_Ligada == 1) { // se a bomba estiver ligada
        if (SINAL_ESTAC_LIGADO) { // permanece verificando se a mesma nao desligou
            shift[rl_motor_arranque] = 0;
            tempo_limite_partida = 0;
        } else {
            estacionaria_Ligada = 0; // Se a  bomba nao ligou, ou desligou no periodo, tentar ligar novamente, até estourar o tempo maximo de partida
        }
    }
}

void acaoTesteBomasProgramado() {
    if (dias_restante_teste >= intervalo_teste_dia && intervalo_teste_dia != 0) { // Se ajustado intervalo como 0 entao desativa a funçao de teste automatico
        if (_hor == horario_de_teste) {
            dias_restante_teste = 0;
            writeEEPROM_ext(10, dias_restante_teste);
            teste_run = 2;
            menu_posi = 28;
            acaoTesteBombas();
        }
    }
}

void acaoTesteBombas() {
    if (btPress(b_esc)) {
        teste_run = 0;
        tempo_teste_bombas = 300;
        menu_posi = _menu_posi;
        char i = 0;
        for (i = 0; i < 8; i++) {
            shift[i] = 0;
        }
    }
    if (_sec_aux != _sec) { // temporizador para tempo limite de teste; // Talvez precise colocar( && shift[rl_sol_despressurizacao] == 0)
        _sec_aux = _sec;
        tempo_teste_bombas--;
        if (tempo_teste_bombas == 250) { // 50 segundos
            teste_run = 5; // jockey
            status_jockey = ERRO;
            shift[rl_jockey] = 0;
        } else if (tempo_teste_bombas == 170) { // 80 segundos
            teste_run = 8; // principal
            status_principal = ERRO;
            shift[rl_principal] = 0;
        } else if (tempo_teste_bombas == 1) { //169 segundos
            teste_run = 12; // estacionaria
            status_estacionaria = ERRO;
            partidaBombaEstacionaria(0); // desligar
        }
    }
    if (teste_run < 12) {
        sprintf(line1, "   Aguarde.. %3dsec", tempo_teste_bombas);
        sprintf(line2, " Pressao %s bar", intToFloatStr(pressao));
    }
    switch (teste_run) {
        case 2:
            dias_restante_teste = 0; // ZERA CONTADOR DE DIAS POR TESTE AUTOMATICO
            writeEEPROM_ext(10, dias_restante_teste);
            shift[rl_sol_despressurizacao] = 1;
            teste_run = 3;
            break;
        case 3:
            if (pressao <= (sp_pressao_rede - 10)) { // - 1BAR abaixo do setpoint do sistema
                shift[rl_sol_despressurizacao] = 0;
                shift[rl_jockey] = 1;
                teste_run = 4;
            }
            break;
        case 4:
            if (pressao >= sp_pressao_rede) {
                shift[rl_jockey] = 0;
                status_jockey = OK;
                teste_run = 5;
                tempo_teste_bombas = 249;
            }
            break;
        case 5:
            shift[rl_sol_despressurizacao] = 1;
            teste_run = 6;
            break;
        case 6:
            if (pressao <= (sp_pressao_rede - 10)) {
                shift[rl_principal] = 1;
                shift[rl_sol_despressurizacao] = 0;
                teste_run = 7;
            }
            break;
        case 7:
            if (pressao >= sp_pressao_rede) {
                shift[rl_principal] = 0;
                status_principal = OK;
                teste_run = 8;
                tempo_teste_bombas = 169;
            }
            break;
        case 8:
            shift[rl_sol_despressurizacao] = 1;
            teste_run = 9;
            break;
        case 9:
            if (pressao <= (sp_pressao_rede - 10)) {
                shift[rl_sol_despressurizacao] = 0;
                teste_run = 10;
            }
            break;
        case 10:
            partidaBombaEstacionaria(1); // Liga a bomba a combustao
            if ((tempo_coun_estacionaria != _sec) && estacionaria_Ligada == 1) { // soma tempo de teste quando tiver ligada
                tempo_coun_estacionaria = _sec;
                tempo_corrido_teste_estacionaria++;
            }
            if (tempo_corrido_teste_estacionaria >= tempo_limite_teste_estacionaria) {
                status_estacionaria = OK;
                tempo_corrido_teste_estacionaria = 0;
                teste_run = 11;
            }
            if (status_estacionaria == ERRO || falha == 1) {
                teste_run = 11;
                status_estacionaria = ERRO;
            }
            break;
        case 11:
            salvarLOG();
            clearShiftREG();
            bt_press = 1; // força limpar o lcd
            teste_run = 12;
            tempo_teste_bombas = 1;
            break;
        case 12:
            sprintf(line1, "  TESTE CONCLUIDO!");
            sprintf(line2, " JOKEY : %s", status_jockey == 1 ? "OK" : "ERRO");
            sprintf(line3, " PRINCIPAL : %s", status_principal == 1 ? "OK" : "ERRO");
            sprintf(line4, " COMBUSTAO : %s", status_estacionaria == 1 ? "OK" : "ERRO");
            //            enviaSMS(2); // sms de teste;;
            if (tempo_teste_bombas <= -50) { // 50 segundos mostrando a tela.
                tempo_teste_bombas = 300;
                menu_posi = 0;
                teste_run = 0;
                bt_press = 1; // força limpar o lcd
            }
            break;
        default:
            teste_run = 0;
            menu_posi = 0;
            break;
    }
}

#endif

