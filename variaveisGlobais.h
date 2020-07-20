#ifndef VARIAVEISGLOBAIS_H
#define	VARIAVEISGLOBAIS_H
#include <stdbool.h>

#define ICONE_TORRE_SINAL 0x01 
#define ICONE_SINAL_FRACO 0x02
#define ICONE_SINAL_MEDIO 0x03 
#define ICONE_SINAL_BOM 0x04
#define ICONE_TOMADA 0x05

#define INTERVALO_ENVIO_SMS 100 // em segundos
#define TEMPO_DURACAO_CHAMADA 35 // em segundos, tempo para realizar a chamada

#define ERRO 0
#define OK 1

#define SINAL_ESTAC_LIGADO !PORTAbits.RA2
#define SINAL_AUX2 !PORTAbits.RA5
#define FALTA_ENERGIA !PORTCbits.RC0

#define out_RL_JOCKEY LATAbits.LATA3
#define out_RL_PRINCIPAL LATDbits.LATD7
#define out_RL_ESTACIONARIA LATEbits.LATE0
#define out_RL_SOLENOIDE1 LATEbits.LATE1
#define out_RL_SOLENOIDE2 LATEbits.LATE2
#define out_RL_ALARME LATCbits.LATC1

unsigned char out_BOMBA_ESTACIONARIA = 0;

unsigned char line1[21]; // linhas do lcd
unsigned char line2[21]; // linhas do lcd
unsigned char line3[21]; // linhas do lcd
unsigned char line4[21]; // linhas do lcd

unsigned char tentativasVerificarGSM = 0;
unsigned char intensidadeSinal = ' ';
unsigned char telefoneSelecionado = 0;

unsigned char timerIntervaloEntreBombas = 10;
unsigned char timerVerificarSinal = 5;
unsigned char timerReenvioSMS = 0;
unsigned char timerEtapaComunicacao = 0;
unsigned char timerTrocarTextoLcd = 0;
unsigned char timerAtrasoLCD = 0;
unsigned char timerBase = 0;
unsigned char timerPiscarLCD = 0;
unsigned char timerColetaLeitura = 0;
unsigned int timerTesteBombas = 0;

unsigned int etapaComunicacao = 0;
bool gsmOcupado = false;
bool textoAlternativo = false;

unsigned char codigoErro = 0;
unsigned char ocorrendoIncendio = 0;
unsigned char flagErroTransdutor = 0;

unsigned bool habilitaTesteAutomatico = 0;
unsigned char etapaTesteBombas = 0;
unsigned char ultimoEstadoSaidas = 0;
long ultimoEstadoLCD = 100;

unsigned int horimetro = 0;

unsigned char _sec = 0;
unsigned char _min = 0;
unsigned char _hor = 0;
unsigned char _dia = 0;
unsigned char _mes = 0;
unsigned char _ano = 0;
unsigned char intervalo_teste_dia = 15;
unsigned char dias_restante_teste = 0;
unsigned char horario_de_teste = 0;

int pressao = 0;
int coletaMedias[5] = {0, 0, 0, 0, 0};
unsigned char posicaoColetaLeitura = 0;

unsigned bool status_jockey = true, status_principal = true, status_estacionaria = true;
unsigned char tempo_limite_estacionaria = 0, tempo_partida = 0, tempo_limite_partida = 0, tempo_limite_teste_estacionaria = 0;
unsigned char sp_pressao_rede = 0, sp_jockey = 0, sp_principal = 0, sp_estacionaria = 0;
unsigned char option_posi = 0, menu_posi = 0, _menu_posi = 0, sub_menu_posi = 0, tela_menu_posi = 0;
unsigned char tel_posi = 0, ajust_tel = 0;
unsigned bool ajst_rtc = 0, blinkk = 0;

char tel1[12] = {'\0'};
char tel2[12] = {'\0'};

#endif

