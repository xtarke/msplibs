/*
 * 03_main_ext_irq.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#define LED   BIT0
#define BUTTON_0  BIT1

void config_ext_irq(){
    /* Primeiramente configura porta:
     *
     * Cuidado quando há entrada e saídas
     *
     * Todos pinos como entrada  */
    P4DIR = 0x00;

    /* Pull up/down */
    P4REN = BUTTON_0;

    /* Pull up */
    P4OUT = BUTTON_0;

    /* Habilitação da IRQ apenas botão */
    P4IE =  BUTTON_0;

    /* Transição de nível alto para baixo */
    P4IES = BUTTON_0;

    /* Limpa alguma IRQ pendente */
    P4IFG &= ~BUTTON_0;
}

void main(){
    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Configura port do LED */
    P1DIR = LED;


    /* Configura interupções */
    config_ext_irq();

    /* Habilita IRQs e desliga CPU */
    __bis_SR_register(LPM4_bits | GIE);

}



