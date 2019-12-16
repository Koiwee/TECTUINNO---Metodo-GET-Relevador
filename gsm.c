#include<msp430.h>



/*
 * tectuino_ini_gsm.c
 *
 *  Created on: 10/07/2019
 *      Author: ThinkPad
 */


void config_uart(void){
	  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	  P1SEL2 |= BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
	  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	  UCA0BR0 = 0x41;//	  UCA0BR0 = 104; para 1MHz 9600   0x41 para 8mhz
	  UCA0BR1 = 3;//	  UCA0BR1 = 0;  para 1MHz 9600  y 3 para 8mhz
	  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1    UCBRS6 para 115200
	  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	  //IE2 &= ~UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}


void config_botones(void){
	  P1IES &= ~(BIT3+BIT4);                            // P1.3 Hi/lo edge
	  P1REN |= BIT3+BIT4;							// Enable Pull Up on SW2 (P1.3)
	  P1OUT |= BIT3+BIT4;
	  P1IFG &=~(BIT3+BIT4);                           // P1.3 IFG cleared   //BIT3 on Port 1 can be used as Switch2
      P1IE |=  BIT3+BIT4;                            // P1.3 interrupt enabled

}

void config_sensores(void){
	  P2REN |= BIT3+BIT0+BIT2;							// Enable Pull Up on SW2 (P1.3)
      P2OUT |= BIT3+BIT0+BIT2;
	  P2IFG &= ~(BIT3+BIT0+BIT2);                           // P1.3 IFG cleared   //BIT3 on Port 1 can be used as Switch2
	  P2IES |= (BIT3+BIT0+BIT2);                            // P1.3 Hi/lo=0   Low/hig=1 edge
	  P2IE |=  BIT3+BIT0+BIT2;                            // P1.3 interrupt enabled
}
void config_buzzer(void){
	  P1DIR |=BIT0+BIT5; // LED D4 P1.0  BUZZER P1.5   TODOS SE ENCIEDEN CON 0
	  P1OUT |=BIT0+BIT5;  // APAGADOS
	  P2DIR |=BIT1; // CONFIGURA SALIDA P2.1 RESET DEL GSM
}


void config_timer0(void){
	TA0CCTL0 = CM_0 | CCIS_0 | OUTMOD_0 ;
	TA0CCTL0 |=CCIE;
	TA0CCR0 = 50000;  //XTAL
	TA0CTL = TASSEL_2 | ID_3 | MC_1 |TACLR;
	TA0CTL &=~ MC_1; // apaga el timer
}


void config_timer1(void){
	TA1CCTL0 = CM_0 | CCIS_0 | OUTMOD_0 | CCIE;
	TA1CCR0 = 50000;  //
	TA1CTL = TASSEL_2 | ID_3 | MC_1;
	TA1CTL &=~ MC_1; // apaga el timer
}


void enviar_com_AT(char *una_tu_cadena)
{
	int pozol =0;
	while (una_tu_cadena[pozol] != 0x00)
	{
		 while (!(IFG2&UCA0TXIFG));
		  UCA0TXBUF=una_tu_cadena[pozol];
		 	_delay_cycles(8000);
		pozol++;
	}
}

int contactar_a(int conchacto, int llama_x){
	 // posiXY(1,0);
	//enviar_OLED("                         ");

	if(conchacto==0){
//		  posiXY(2,0);
    	enviar_OLED("BUSCAR CONTAC",0,0);
  //  	  posiXY(0,0);
   // 	enviar_OLED("ENCONTRAR EL CONTACTO    ");
				}

if(conchacto==1){
    	enviar_OLED("DIEGO TORRES",0,0);
		enviar_OLED("9611121331",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9611121331;\r\n");
	  	llama_x=0;

    	}}


if(conchacto==2){
    	enviar_OLED("ZAHIRA COUTINO",0,0);
    	enviar_OLED("961 1110688",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9611110688;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==3){
    	enviar_OLED("WENDY TEC",0,0);
    	enviar_OLED("55 17701290",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD5517701290;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==4){
    	enviar_OLED("EDUARDO AGREDA",0,0);
    	enviar_OLED("962 6924874",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9626924874;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==5){
    	enviar_OLED("JHOVANY TEC",0,0);
    	enviar_OLED("961 2785583",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9612785583;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==6){
    	enviar_OLED("LAURENT REYES",0,0);
    	enviar_OLED("961 1420897",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9611420897;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==7){
    	enviar_OLED("MEXLED OFICI",0,0);
    	enviar_OLED("961 6885873",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9616885873;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==8){
    	enviar_OLED("CONSUELO ROB",0,0);
    	enviar_OLED("961 1821944",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9611821944;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==9){
    	enviar_OLED("RAUL TORRES ",0,0);
    	enviar_OLED("961 2394236 ",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9612394236;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==10){
    	enviar_OLED("OSVALDO HERN",0,0);
    	enviar_OLED("961 2213157",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9612213157;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==11){
    	enviar_OLED("LUISIN MENDE",0,0);
    	enviar_OLED("961 2400583",2,0);
    	if(llama_x==1){
	  	enviar_com_AT("ATD9612400583;\r\n");
	  	llama_x=0;

    	}}

if(conchacto==12){
    	enviar_OLED("MANUELITA   ",0,0);
    	enviar_OLED("961 2448311",2,0);
    	if(llama_x==1){
    		enviar_com_AT("ATD9612448311;\r\n");
	  	llama_x=0;

    	}}

	_delay_cycles(30000);

return llama_x;
}


void reset_gsm(void){
	P2OUT &=~BIT1;    // P2.1  ---__----  0000 0000
	_delay_cycles(400000); //	50mS
	P2OUT |=BIT1;     // P2.1  ----------  0000 0010
	//_delay_cycles(32000000);  // CHANCE QUE SE RECUPERE 4 segundos

}
