/*
 * 03_main_ext_irq.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#define LED_1   BIT0
#define BUTTON_0  BIT3

void config_ext_irq(){
    /* Primeiramente configura porta: LEDS como saída.
     * Demais pinos como entraada  */
    P1DIR = LED_1;

    /* Pull up/down */
    P1REN = BUTTON_0;

    /* Pull up */
    P1OUT = BUTTON_0;

    /* Habilitação da IRQ apenas botão */
    P1IE =  BUTTON_0;

    /* Transição de nível alto para baixo */
    P1IES = BUTTON_0;

    /* Limpa alguma IRQ pendente */
    P1IFG &= ~BUTTON_0;
}



void main(){
    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

    /* Configura interupções */
    config_ext_irq();

    /* Habilita IRQs e desliga CPU */
    __bis_SR_register(LPM4_bits | GIE);

}


/* Port 1 ISR (interrupt service routine) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    /* Liga/desliga LED quando detectado borda no botão */
    P1OUT ^= LED_1;

    /* Limpa sinal de IRQ do botão 0 */
    P1IFG &= ~BUTTON_0;
}
