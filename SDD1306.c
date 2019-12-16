#include<msp430.h>
#include "tabla_wtr_ascii2.h"

/*
 * 0000_ini_oled.c
 *
 *  Created on: 10/07/2019
 *      Author: ThinkPad
 */
void enviar_config_oled(const unsigned char *, const unsigned char);

const unsigned char comandos_config[] ={0xAE,0X00,0X10,0X40,0X81,0X3F,0XA1,0XA6,0XA8,0X3F,0XD3,0X00,0XD5,0X80,0XD9,0XF1,0XDA,0X12,0XDB,0X40,0X8D,0X14,0XAF,0XA4,0XB0,0X00,0X10};


void Reset_Oled(void){
 	 P1OUT |=BIT6;
     P1DIR |=BIT6;
	 P1OUT &=~BIT6;
	 _delay_cycles(300000);  // RESET AL CLK DEL OLED SDD1306
	 P1OUT |=BIT6;
	 P1DIR &=~BIT6;
	 P1OUT &=~BIT6;	}


void ini_SDD1306(void){
	WDTCTL = WDTPW + WDTHOLD;
	Reset_Oled();
	P1SEL |= BIT6 + BIT7;                  // bit de datos del I2C
    P1SEL2 |= BIT6 + BIT7;                  //bit de clk para el I2c
    enviar_config_oled(comandos_config,27);  // CADENA QUE INTEGRA TODOS LOS BYTES DE CONFIGURACION AL OLED (COMANDOS)
}


void byte_bit_inicio(void){
    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCMODE_3 + UCMST + UCSYNC;           // I2C master mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // Use SMCLK, keep SW reset
    UCB0BR0 = 0x40;                          //40h   --- < 100 kHz
    UCB0I2CSA = 0x3C;                       // address
    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0TXIE;                // Enable TX ready interrupt
    __disable_interrupt();
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition

    __bis_SR_register(LPM3_bits + GIE);;
}

void bit_stop(void){
    UCB0CTL1 |= UCTR + UCTXSTP;
    IE2 &= ~UCB0TXIE;
}

void enviar_config_oled(const unsigned char *cadena_config, const unsigned char cant_comm){
	byte_bit_inicio();

    unsigned int c;
    for(c=0;c<cant_comm;c++){
        UCB0TXBUF = 0x80;
        __bis_SR_register(LPM3_bits + GIE);
        UCB0TXBUF = cadena_config[c];
        __bis_SR_register(LPM3_bits + GIE);
    }
    bit_stop();
}


void comando_oled(unsigned char c){
	byte_bit_inicio();   // condición START
    UCB0TXBUF = 0x80;
    __bis_SR_register(LPM3_bits + GIE);
    UCB0TXBUF = c;
    __bis_SR_register(LPM3_bits + GIE);
    bit_stop();   // condición STOP
    __delay_cycles(800);
}

void ini_fila(unsigned char d){
	comando_oled(0xb0 | d);
}

void ini_columna(unsigned char d){
	comando_oled(0x00+d%16);
	comando_oled(0x10+d/16); //set higher column start address
}

void dato_oled(const unsigned char Data){
    UCB0TXBUF = Data;
    __bis_SR_register(LPM3_bits + GIE);
}


void limpia_oled(unsigned char Data){
	unsigned char i,j;
	for(i=0;i<8;i++){
		ini_fila(i);
		ini_columna(0x00);

		byte_bit_inicio();   // condición START
        UCB0TXBUF = 0x40;
	  	__bis_SR_register(LPM3_bits + GIE);

		for(j=0;j<128;j++){
			dato_oled(Data);
		}
	    bit_stop();   // condición STOP
	}
}

void imagen(unsigned char *ima){
	unsigned char i,j,x=0;
	for(i=0;i<8;i++){
		ini_fila(i);
		ini_columna(0x00);
		byte_bit_inicio();   // condición START
        UCB0TXBUF = 0x40;
	  	__bis_SR_register(LPM3_bits + GIE);
		for(j=0;j<128;j++){
	    	dato_oled(ima[x*128+j]);

		}
		x++;
	    bit_stop();   // condición STOP
	}
}

void enviar_car(unsigned char Data, char offset){
	unsigned char i,j;
		if (Data<' ' || Data>'~'){Data=' ';}

		Data=Data-' '; // ESPACIO EL PRIMER CARACTER ASCII

		byte_bit_inicio();   // condición START
        UCB0TXBUF = 0x40;
	  	__bis_SR_register(LPM3_bits + GIE);
		for(j=0;j<10;j++){
			i=letras[Data*20+j+ offset];
			dato_oled(i);
		}
	    bit_stop();   // condición STOP
}

void posiXY(int x, int y){
    ini_fila(x);  // número de fila (0-7)
    ini_columna(y);// columna (0-127)
}

void enviar_OLED(char cadenaTNM[],char x, char y)
{
	posiXY(x,y);
	int apunta =0;
		while (cadenaTNM[apunta] != 0x00)
		{
			enviar_car(cadenaTNM[apunta],0);
			_delay_cycles(800);
			apunta++;
		}

		posiXY(x+1,y);
		 apunta =0;
		while (cadenaTNM[apunta] != 0x00)
		{
			enviar_car(cadenaTNM[apunta],10);
			_delay_cycles(800);
			apunta++;
		}
}

void enviar_MATRIX_OLED(const unsigned char filade_car[], int offset1)
{
	int col =0;
	for(col=0;col<25;col++)
	{
		enviar_car(filade_car[col+offset1],0);
	}
}

void mensaje_ini(void){
	//posiXY(7,0);
	enviar_OLED("-------------------------",7,0);

	//posiXY(6,0);
	enviar_OLED("[ TECNOLOGICO NACIONAL  [",6,0);

	//posiXY(5,0);
	enviar_OLED("[-----------------------[",5,0);

	//posiXY(4,0);
	enviar_OLED("[  DE MEXICO: TEC TUXTLA[",4,0);

	//posiXY(3,0);
	enviar_OLED("[-----------------------[",3,0);
}

void imagen_ascii(const unsigned char *x){
	posiXY(7,0);
	enviar_MATRIX_OLED(x,26*0);

	posiXY(6,0);
	enviar_MATRIX_OLED(x,26*1);

	posiXY(5,0);
	enviar_MATRIX_OLED(x,26*2);

	posiXY(4,0);
	enviar_MATRIX_OLED(x,26*3);

	posiXY(3,0);
	enviar_MATRIX_OLED(x,26*4);

	posiXY(2,0);
	enviar_MATRIX_OLED(x,26*5);

	posiXY(1,0);
	enviar_MATRIX_OLED(x,26*6);

	posiXY(0,0);
	enviar_MATRIX_OLED(x,26*7);

}


