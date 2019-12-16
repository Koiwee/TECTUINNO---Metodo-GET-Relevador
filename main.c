#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "varios.h"
#include "gsm.h"
#include "logo_tnm.h"
#include "SDD1306.h"

#include "gprs.h"

void arranque(void);
void limpiar_ram(void);
void recuperar_respuesta(void);

int i=0,ctr=0;
char ram[150]={0,0,0};          //Espacio de memoria para respuestas del módem (SIM800L)
char respuestaGET[150]={0,0,0}; //Espacio de memoria para la respuesta del GET

int main(void) {
    arranque();

    while(1){}
}


// proceso de interrupción DE tx DE la I2C  *****
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
    IFG2 &= ~UCB0TXIFG;  //Limpia bandera después una vez que entra a la interrupción
    __bic_SR_register_on_exit(LPM3_bits); // da la instrucción al procesador de mantenerse despierto
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    ram[i++] = UCA0RXBUF;                    // TX -> RXed character
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
  if(P1IFG & BIT3){
      P1IFG &= ~BIT3;
      recuperar_respuesta();
  }
}

void arranque(void){
    WDTCTL = WDTPW | WDTHOLD;
	config_osc();
    ini_SDD1306();
    config_uart();

    P1DIR |= BIT0;  //P1.0 LED
    P2DIR |= BIT3;  //P2.3 Relay

    P1REN = BIT3;
    P1OUT |= BIT3;
    P1IE = BIT3;

    P1OUT |= BIT0;
    P2OUT |= BIT3;

 	comando_oled(0XC8);
    imagen(logotnm);
    _delay_cycles(4000000*8);
    limpia_oled(0);
    limpiar_ram();

    __bis_SR_register(GIE);       // Entra al modo ahorro LPM0(se duerme el uC)
}

void limpiar_ram(void){
    int x;
    for( x=0 ; x<150 ; x++ ){
        ram[x]=0;
    }
    i=0;
}

void recuperar_respuesta(void){
    enviar_OLED("RECUPERAR",2,0);
    _delay_cycles(1000000*8);
    limpia_oled(0);
    verificacion_CREG(ram);

    iniciar_GPRS(ram, "internet.itelcel.com");

    if(verificar_IP(ram)){
        int x;
        for(x=0;x<150;x++) respuestaGET[x] = 0;

        metodo_GET(ram,respuestaGET,"http://jsonplaceholder.typicode.com/todos/1");

        //Encontrar posición de los valores en la cadena. Se almacenan en val1 y val2.
        char *val1 = strstr(respuestaGET,"id\":") + 5;
        char *val2 = strstr(respuestaGET,"completed\":") + 12;

        if( val1 != NULL){
            //Encontrar una posición donde terminará val1
            char *fin_val1 = strchr(val1,',');
            //Delimitar la cadena con un 0
            *fin_val1 = 0;

            enviar_OLED("Caso 1:",2,0);
            //val1 se imprimirá en OLED hasta hallar un 0 (fin_val1)
            enviar_OLED(val1,4,0);
            _delay_cycles(1000000*8*2);
            limpia_oled(0);

            //Si dentro de val1 hay un '1'
            if( strchr(val1,'1') != NULL){
                //Activar el Relevador
                P2OUT &= ~BIT3;

                //Dos parpadeos del LED
                for( x=0 ; x<2 ; x++ ){
                    P1OUT &= ~BIT0;
                    _delay_cycles(1000000*6);
                    P1OUT |= BIT0;
                    _delay_cycles(1000000*6);
                }
            //Si no
            } else {
                //Desactivar el Relevador
                P2OUT |= BIT3;

                //Un parpadeo del LED
                P1OUT &= ~BIT0;
                _delay_cycles(1000000*6);
                P1OUT |= BIT0;
            }
        }

        if( val2 != NULL){
            //Encontrar una posición donde terminará val2
            char *fin_val2 = strchr(val2,'}');
            //Delimitar la cadena con un 0
            *fin_val2 = 0;

            enviar_OLED("Caso 2:",2,0);
            //val2 se imprimirá en OLED hasta hallar un 0 (fin_val2)
            enviar_OLED(val2,4,0);
            _delay_cycles(1000000*8*2);
            limpia_oled(0);

            //Si dentro de val2 hay una 't' (true)
            if( strchr(val2,'t') != NULL){
                //Activar el Relevador
                P2OUT &= ~BIT3;
                //Dos parpadeos del LED
                for( x=0 ; x<2 ; x++ ){
                    P1OUT &= ~BIT0;
                    _delay_cycles(1000000*6);
                    P1OUT |= BIT0;
                    _delay_cycles(1000000*6);
                }

            //Si no, si dentro de val2 hay una 'f' (false)
            } else if(strchr(val2,'f') != NULL){
                //Desactivar el Relevador
                P2OUT |= BIT3;

                //Un parpadeo del LED
                P1OUT &= ~BIT0;
                _delay_cycles(1000000*6);
                P1OUT |= BIT0;
            }
        }
        cerrar_GPRS(ram);
    }
}
