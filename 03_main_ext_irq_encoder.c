/*
 * 03_main_ext_irq_encoder.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>
#include <stdint.h>

#define LED_DEBUG   BIT0
#define ENCODER_INPUT  BIT3

volatile uint16_t pulses = 0;

void config_ext_irq(){
    /* Primeiramente configura porta: LEDS como saída.
     * Demais pinos como entraada  */
    P1DIR = LED_DEBUG;

    /* Pull up/down */
    P1REN = ENCODER_INPUT;

    /* Pull up */
    P1OUT = ENCODER_INPUT;

    /* Habilitação da IRQ apenas botão */
    P1IE =  ENCODER_INPUT;

    /* Transição de nível alto para baixo */
    P1IES = ENCODER_INPUT;

    /* Limpa alguma IRQ pendente */
    P1IFG &= ~ENCODER_INPUT;
}



void main(){
    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Configura interupções */
    config_ext_irq();

    /* Habilita IRQs e desliga CPU */
    __bis_SR_register(LPM4_bits | GIE);

    /* Este laço nunca é executado nesse exemplo */
    while (1){
        //(...)

        /* Descomente essa linha caso deseje desligar CPU novamente */
        //__bis_SR_register(LPM4_bits);
    }

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
    /* Liga/desliga LED quando detectado borda no encoder */
    P1OUT ^= LED_DEBUG;

    /* Conta o número de pulsos */
    pulses++;

    /* Limpa sinal de IRQ do botão 0 */
    P1IFG &= ~ENCODER_INPUT;

    /* Essa linha deve ser descomentada para o main executar */
    //  __bic_SR_register_on_exit(LPM4_bits);
}
