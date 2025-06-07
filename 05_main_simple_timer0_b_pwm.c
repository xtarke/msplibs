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
 *      /|\
 *       |
 *       |
 *       |             +  <-- Comparador 0 (Contágem máxima do Timer (período do PWM)
 *       |           +
 *       |         +
 *       |       + <--- Comparador 1 (TACCR1: canal PWM 1)
 *       |     +
 *       |   +  <----  Comparador 2 (TACCR1: canal PWM 2)
 *       | +
 *       +----------------------------------------->
 *
 *       - Comparadores podem ser configurados para qualquer valor
 *       entre 0 e 65535.
 *
 */

#ifndef __MSP430FR2355__
#error "Example not supported for this device"
#endif

/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/gpio.h"

#define LED_1 BIT0
#define LED1_PORT P1

#define LED2_PORT P6
#define LED_2 BIT6

/**
 * @brief  Configura temporizador tipo B como PWM (contágem até CCR0)
 *
 * @param  none
 *
 * @retval none
 */
void config_timerB_0_as_pwm(){
    /* Timer A0:
     *
     * TBSSEL_2 -> Clock de SMCLK (clock da CPU: 1MHz se não alterado).
     * MC_1 -> Contagem crescente até CCR0
     * ID_0 -> Prescaler = /1
     */
    TB0CTL = TBSSEL_2 | MC_1 | ID_0;

    /* Contagem máxima do timer: modo MC1, conta até CCR0 */
    TB0CCR0 = 45000;

    /* Configura canal 1 do PWRM */
    TB0CCTL1 = OUTMOD_7;
    TB0CCR1 = 5000;     // Razão cíclica inícial (relação CCR1/CCR0)
    /* Canal 1 do PWM está em P1.6 */
    P1DIR |= BIT6;      // Configura P1.6 como saída
    P1SEL1 |= BIT6;      // Conceta CCR1 no pino P1.6

    /* Configura canal 1 do PWRM */
    TB0CCTL2 = OUTMOD_7;
    TB0CCR2 = 10000;     // Razão cíclica inícial (relação CCR2/CCR0)
    /* Canal 2 do PWM está em P1.7 */
    P1DIR |= BIT7;      // Configura P1.7 como saída
    P1SEL1 |= BIT7;      // Conceta CCR1 no pino P1.7

}

/**
 * @brief  Configura razão cíclia do comparador ccr1
 *
 * @param  value
 *
 * @retval none
 */
void change_duty_ccr1(uint16_t value){
    TB0CCR1 = value;
}

/**
 * @brief  Configura razão cíclia do comparador ccr2
 *
 * @param  value
 *
 * @retval none
 */
void change_duty_ccr2(uint16_t value){
    TB0CCR2 = value;
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
    config_timerB_0_as_pwm();

    /* Entra em modo de economia de energia com IRQs habilitadas */
    __bis_SR_register(LPM0_bits + GIE);
}


