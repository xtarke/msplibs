/*
 * 05_main_simple_wd_timer.c
 *
 *  Created on: Mar 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */


#ifndef __MSP430FR2355__
    #error "Clock system not supported for this device"
#endif

/* System includes */
#include <msp430.h>

/* Project includes */
#include "lib/gpio.h"

#define LED BIT0
#define LED_PORT P1


/**
  * @brief  Configura temporizador watchdog.
  *
  * @param  none
  *
  * @retval none
  */
void config_wd_as_timer(){
    /* Configura Watch dog como temporizador:
     *
     * WDT_ADLY_1000 <= (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)
     *
     * WDTPW -> "Senha" para alterar confgiuração.
     * WDTTMSEL -> Temporizador ao invés de reset.
     * WDTSSEL -> Fonte de clock de ACLK
     * WDTIS2 -> Período de 1000ms com ACLK = 32.768Hz
     *
     */
    WDTCTL = WDT_ADLY_1000;
    /* Ativa IRQ do Watchdog */
    SFRIE1 |= WDTIE;
}


int main(void)
{
    /* Desliga watchdog imediatamente */
    WDTCTL = WDTPW | WDTHOLD;
    
#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Configurações de hardware */
    config_wd_as_timer();

    PORT_DIR(LED_PORT) = BIT0;

    /* Entra em modo de economia de energia */
    __bis_SR_register(LPM0_bits + GIE);

}


/* ISR do watchdog: executado toda a vez que o temporizador estoura */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    PORT_OUT(LED_PORT) ^= LED;
}
