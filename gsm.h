/*
 * tectuino_ini_gsm.h
 *
 *  Created on: 10/07/2019
 *      Author: ThinkPad
 */

#ifndef GSM_H_
#define GSM_H_


void config_uart(void);
void config_botones(void);
void config_sensores(void);
void config_buzzer(void);
void config_timer0(void);
void config_timer1(void);
void enviar_com_AT(char *una_tu_cadena);
int contactar_a(int conchacto, int llama_x);


#endif /* GSM_H_ */
