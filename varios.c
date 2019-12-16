#include<msp430.h>
/*
 * varios.c
 *
 *  Created on: 10/07/2019
 *      Author: ThinkPad
 */


void config_osc(void){
	  if (CALBC1_8MHZ==0xFF)					// If calibration constant erased
	  {
	    while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
	  DCOCTL = CALDCO_8MHZ;
}



void ring_buzzer(void){
int i;
	for(i=0;i<7;i++){
			 P1OUT &=~BIT5;
			 _delay_cycles(400000);
		  P1OUT |=BIT5;
			 _delay_cycles(160000);}}





void habilita_ints(void){
    __bic_SR_register(GIE);       // Entra al modo ahorro LPM0(se duerme el uC)

	IFG2 &=~UCA0RXIFG;
	P1IFG &=~(BIT3+BIT4);                           // P1.3 IFG cleared   //BIT3 on Port 1 can be used as Switch2
	P2IFG &= ~(BIT3+BIT0+BIT2);                           // P1.3 IFG cleared   //BIT3 on Port 1 can be used as Switch2


    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

	P1IE |=  BIT3+BIT4;                            // P1.3 interrupt enabled
    P2IE |=  BIT3+BIT0+BIT2;                            // P1.3 interrupt enabled

	TA0CTL |= TACLR;
	TA0CTL |= MC_1; // on timer
	TA0CCTL0 |=CCIE;

	TA1CTL |= TACLR;
	TA1CTL |= MC_1; // on timer
	TA1CCTL0 |=CCIE;

	__bis_SR_register(GIE);       // Entra al modo ahorro LPM0(se duerme el uC)

}

void deshabilita_ints(void){
    __bic_SR_register(GIE);       // Entra al modo ahorro LPM0(se duerme el uC)

    IE2 &= ~UCA0RXIE;                          // Enable USCI_A0 RX interrupt

	P1IE &= ~BIT3+BIT4;                            // P1.3 interrupt enabled
    P2IE &= ~BIT3+BIT0+BIT2;                            // P1.3 interrupt enabled

	TA0CTL &= ~TACLR;
	TA0CTL &= ~MC_1; // on timer
	TA0CCTL0 &= ~CCIE;

	TA1CTL &= ~MC_1; // on timer
	TA1CCTL0 &= ~CCIE;

    __bis_SR_register(GIE);       // Entra al modo ahorro LPM0(se duerme el uC)
}
