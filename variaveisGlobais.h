#ifndef VARIAVEISGLOBAIS_H
#define	VARIAVEISGLOBAIS_H

#define ICONE_TORRE_SINAL 0x01 
#define ICONE_SINAL_FRACO 0x02
#define ICONE_SINAL_MEDIO 0x03 
#define ICONE_SINAL_BOM 0x04


#define INTERVALO_ENVIO_SMS 2 // em minutos
#define TEMPO_ENVIO_SMS 20 // em segundos, tempo de espera para enviar cada sms
#define TEMPO_DURACAO_CHAMADA 30 // em segundos, tempo para realizar a chamada
#define ATRASO_ARRANQUE_COMBUSTAO 700 // em ms, tempo em que o motor de arranque continua após ligar a bomba 


#define ERRO 0
#define OK 1

#define SINAL_ESTAC_LIGADO !PORTAbits.RA5
#define SINAL_AUX2 !PORTAbits.RA2
#define FALTA_ENERGIA !PORTCbits.RC0

unsigned char line1[21]; // linhas do lcd
unsigned char line2[21]; // linhas do lcd
unsigned char line3[21]; // linhas do lcd
unsigned char line4[21]; // linhas do lcd

unsigned char base_sec = 0, _sec = 0, _min = 0, _hor = 0, _dia = 0, _mes = 0, _ano = 0, _sec_partida = 0, _sec_aux = 0;
unsigned char intervalo_teste_dia = 0, dias_restante_teste = 0, horario_de_teste = 0;
unsigned char pressao = 98, qtd_leitura = 0, intervalo_leitura_adc = 0;
float _tensao = 0;
unsigned char status_jockey = 1, status_principal = 1, status_estacionaria = 1, status_SIM800L = 0;
int horimetro = 0, tempo_teste_bombas = 300;
unsigned char tempo_limite_estacionaria = 0, estacionaria_Ligada = 0, tempo_partida = 0, tempo_limite_partida = 0, tempo_limite_teste_estacionaria = 0, tempo_corrido_teste_estacionaria = 0, tempo_coun_estacionaria = 0;
unsigned char estagio_envio_SMS = 0, tel_selecionado = 0, teste_sms_run = 0, timer_envio_sms = 0;
unsigned char dtl_pos = 0, dtl_dia = 0, dtl_mes = 0, dtl_ano = 0, dtl_jockey = 0, dtl_principal = 0, dtl_estacionaria = 0, dtl_visualiza_log = 0;
unsigned char tempo_reenvio_SMS = 5; // inicia em 5 minutos
unsigned char sp_pressao_rede = 0, sp_jockey = 0, sp_principal = 0, sp_estacionaria = 0;
unsigned char falha = 0, teste_run = 0, incendio = 0;

unsigned char option_posi = 0, menu_posi = 0, _menu_posi = 0, sub_menu_posi = 0, tela_menu_posi = 0;
unsigned char tel_posi = 0, ajust_tel = 0;
bit ajst_rtc = 0, blinkk = 0;

unsigned char str_recebida[20] = {'\0'};
unsigned char str_posicao = 0, str_recebido_ok = 0, checar_serial = 0;


char tel1[12] = {'\0'};
char tel2[12] = {'\0'};


#endif

