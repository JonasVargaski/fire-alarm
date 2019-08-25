#include <xc.h>
#include "config.h"
#include "registradores.h"
#include "variaveisGlobais.h"
#include "function.h"
#include "transdutor.h"
#include "serial.h"
#include "comunicacao.h"
#include "logs.h"
#include "controle.h"
#include "shift595.h"
#include "temporizador.h"
#include "util.h"
#include "I2C.h"
#include "lcd.h"
#include "ds1307.h"
#include "eeprom.h"
#include "teclado.h"
#include "GSMSIM800L.h"

#include "tela_PRINCIPAL.h"
#include "tela_RELOGIO.h"
#include "tela_TESTE_BOMBAS.h"
#include "tela_DATALOGER.h"
#include "tela_INFORMACOES.h"
#include "tela_TELEFONES.h"
#include "tela_ESTACIONARIA.h"
#include "tela_CONFIG_PRESSAO.h"

#include "tela_TESTE.h"

void interrupt TIMER() {
    if (TMR0IF) { //TIMER 0 OVERFLOW 100ms; InterrupÁ„o;
        TMR0IF = 0x00;
        TMR0H = 0x3C;
        TMR0L = 0xB0;

        base_sec++;

        if (base_sec == 5) {
            blinkk = ~blinkk; // efeito de piscar lcd
        }

        lerTransdutor();
        readButtons();
        
        if (base_sec > 9) { // Base de tempo de 1 segundo
            base_sec = 0;
            blinkk = ~blinkk; // efeito de piscar lcd
            timer();
            if (RCSTAbits.OERR) { // reset da serial caso travar
                RCSTAbits.CREN = 0;
                RCSTAbits.CREN = 1;
            }
        }
    }
    if (RCIF) { //UART Rx interrupÁ„o
        char c = RCREG;
        if (c != '\n' && c != '\r') {
            bufferSerial[cbf++] = c;
        }
        if (cbf >= 60) cbf = 0;
    }
}

void main() {

    configuraRegistradores();
    I2C_Master_Init(100000); //Inicializa I2C modo MASTER em 100KHz clock/
    delay(200);
    RTC_Init(); // Start No DS1307
    delay(100);
    getDS1307TimeDate(&rtc); //sincroniza timer no horario do rtc;
    Lcd_Init(); // Configura TRIS LCD  e inicializa
    Lcd_Clear(); // Limpa oque estiver escrito
    criarCaracteresCGRAM();
    delay(100);
    lerParametros(); // Carrega variuaveis da eeprom.
    sprintf(line1, "TECHNOW");
    sprintf(line2, "Sistemas embarcados");
    sprintf(line3, "Versao Soft.:");
    sprintf(&line4[9], "%s", __DATE__);
    atualizarLCD(line1, line2, line3, line4);
    delay(2500);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (1) {
        asm("CLRWDT"); // Reinicia WDT

        if (!executandoTeste && !flagErroTransdutor) { // se nao tiver em teste das bombas executa.
            acaoBombas();
        }

        verificarIntervaloTesteBombas();

        setShiftREG(); // controle das saidas reles
        getSinalSIM800L(); // pega intensidade do sinal do modulo gsm


        switch (menu_posi) {
            case 0:
                telaPrincipal();
                //                telaTeste();
                break;
            case 1: // 1 tela do menu
                sprintf(&line1[1], "DATA/HORA");
                sprintf(&line2[1], "PRESSAO DA REDE");
                sprintf(&line3[1], "PARTIDA COMBUSTAO");
                sprintf(&line4[1], "TELEFONES");
                break;
            case 2:
                sprintf(&line1[1], "PERIODO DE TESTES");
                sprintf(&line2[1], "DATALOGER");
                sprintf(&line3[1], "INFO VERSAO");
                sprintf(&line4[1], "Iniciar Testes");
                break;
            case 20:
                tela_ajusteRTC();
                break;
            case 21:
                ajusteSP_Bombas();
                break;
            case 22:
                configurarEstacionaria();
                break;
            case 23:
                configuraTelefones();
                break;
            case 24:
                tela_testeBombas();
                break;
            case 25:
                dataLoger();
                break;
            case 27:
                informacoes();
                break;
            case 28:
                acaoTesteBombas();
                break;

            default:menu_posi = 0; // retorna pro inicial se cair em errado;
                break;
        }

        if (menu_posi == 1 | menu_posi == 2) {

            switch (sub_menu_posi) {
                case 1: // RELOGIO/DATA
                    menu_posi = 1;
                    line1[0] = '>';
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 20; // ajuste do rtc
                    }
                    break;
                case 2: // SETPOINT BOMBAS
                    menu_posi = 1;
                    line2[0] = '>';
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 21; // ajuste set point bombas
                    }
                    break;
                case 3: // CONFIG ESTACIONARIA
                    menu_posi = 1;
                    line3[0] = '>';
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 22; // ajuste da bomba estacionaria
                    }
                    break;
                case 4: // TEL/DISCADORA
                    menu_posi = 1;
                    line4[0] = '>';
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 23; // ajuste da discadora / telefones
                    }
                    break;
                case 5:
                    menu_posi = 2;
                    line1[0] = '>';
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 24; // TESTE BOMBAS
                    }
                    break;
                case 6: // DATALOGER
                    line2[0] = '>';
                    menu_posi = 2;
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 25; // data loger
                        dtl_carregar_log = 1; // para abrir o log carregado ja, evitar ficar lendo eeprom direto
                    }
                    break;
                case 7:
                    line3[0] = '>';
                    menu_posi = 2;
                    if (btPress(b_ok)) {
                        _menu_posi = menu_posi;
                        menu_posi = 27; // menu informaÁoes
                    }
                    break;
                case 8:
                    line4[0] = '>';
                    menu_posi = 2;
                    if (btPress(b_ok)) {
                        if (!ocorrendoIncendio) {
                            menu_posi = 28;
                            executandoTeste = true;
                            etapaTesteBombas = 0;
                        }
                    }
                    break;
                default:
                    sub_menu_posi = 1;
                    menu_posi = 1;
                    break;
            }

            if (btPress(b_mais)) { /// NAVEGAÁ√O MENU ///
                sub_menu_posi--;
            } else if (btPress(b_menos)) {
                sub_menu_posi++;
            } else if (btPress(b_esc)) {
                menu_posi = 0;
                option_posi = 0;
                sub_menu_posi = 1;
            }
        }

        if (menu_posi != 28) { // se nao tiver na tela de teste nao interfere no funcionamento
            executandoTeste = false;
            shift[rl_sol_despressurizacao] = 0; // Garante que a solenoide ladr„o nao seja ativada a nao ser que estaja na tela de teste
        }
        atualizarLCD(line1, line2, line3, line4);
    }
}


