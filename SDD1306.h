/*
 * 0000_ini_oled.h
 *
 *  Created on: 10/07/2019
 *      Author: ThinkPad
 */

#ifndef SDD1306_H_
#define SDD1306_H_
void ini_SDD1306(void);
void enviar_MATRIX_OLED(char s[], int offset1);
void enviar_OLED(char cadenaTNM[],char x, char y);
void enviar_car(unsigned char Data);
void imagen(unsigned char *ima);
void Reset_Oled(void);
void mensaje_ini(void);
void limpia_oled(unsigned char Data);
void Reset_Oled(void);
void posiXY(int x, int y);
void comando_oled(unsigned char c);
void imagen_ascii(const char *x);



#endif /* SDD1306_H_ */
