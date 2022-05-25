/*
 * 05_main_simple_timer_a.c
 *
 *  Created on: May 25, 2022
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de configuração Timer Tipo B número 0
 *
 *
 *       .
 *      /|\                  +    <-- Overflow (TABIV_TAIFG - TIMER0_A1_VECTOR)   (2^16)
 *       |                 +
 *       |               +
 *       |             +  <-- Comparador 0 (TACCRB  -> TIMER0_B0_VECTOR)
 *       |           +
 *       |         +
 *       |       + <--- Comparadores 1 e 2 (TACCR1 e TACCR2 - TIMER0_B1_VECTOR)
 *       |     +
 *       |   +
 *       | +
 *       +----------------------------------------->
 *
 *       - Comparadores podem ser configurados para qualquer valor
 *       entre 0 e 65535. IRQs devem ser habilitadas individualmente
 *       nos respectivos registradores.
 *
 */

#ifndef __MSP430FR2355__
#error "Example not supported for this device"
#endif

/* System includes */
#include <msp430.h>

/* Project includes */
#include "lib/gpio.h"

#define LED_1 BIT0
#define LED1_PORT P1

#define LED2_PORT P6
#define LED_2 BIT6

/**
 * @brief  Configura temporizador tipo B.
 *
 * @param  none
 *
 * @retval none
 */
void config_timerB_0(){
    /* Timer A0:
     *
     * TBSSEL_2 -> Clock de SMCLK.
     * MC_2 -> Contagem crescente.
     * TBIE -> Habilitação de IRQ.
     * ID_3 -> Prescaler = /8
     */
    TB0CTL = TBSSEL_2 | MC_2 | ID_3 | TBIE;

    /* IRQ por comparação entre contagem e comparador 0 */
    TB0CCTL0 = CCIE;
    TB0CCR0 = 45000;
}


int main(void)
{
    /* Desliga watchdog imediatamente */
    WDTCTL = WDTPW | WDTHOLD;

    /* Configurações de hardware */
    config_timerB_0();

    PORT_DIR(LED1_PORT) = LED_1;
    PORT_OUT(LED1_PORT) = 0;

    PORT_DIR(LED2_PORT) = LED_2;
    PORT_OUT(LED2_PORT) = 0;

    /* Entra em modo de economia de energia com IRQs habilitadas */
    __bis_SR_register(LPM0_bits + GIE);
}




/* ISR0 do Timer B: executado no evento de comparação  comparador 0 (TBCCR0) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
#else
#error Compiler not supported!
#endif
{
    PORT_OUT(LED1_PORT) ^= LED_1;
}



/* ISR1 do Timer b: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TBxIV_TBIFG))
    {
    /* Vector  0:  No interrupt */
    case  TBxIV_NONE:
        break;

    /* Vector  2:  TACCR1 CCIFG -> Comparação 1*/
    case  TBxIV_TBCCR1:

        break;
    /* Vector  4:  TACCR2 CCIFG -> Comparação 2*/
    case TBxIV_TBCCR2:
        break;

     /* Vector 10:  TAIFG -> Overflow do timer */
    case TBxIV_TBIFG:
        PORT_OUT(LED2_PORT) ^= LED_2;
        break;
    default:
        break;
    }
}
