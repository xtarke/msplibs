/*
 * 01_main_simple_blink.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>
#include <stdint.h>

#define LED   BIT0
#define DELAY 100000

int main(void)
{
    int16_t i;

    /* Desabilita Watch dog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    P1DIR = LED;

    /* main não pode retornar */
    while(1){
        /* Liga/Desliga LED */
        P1OUT = P1OUT ^ LED;

        /* Atraso */
        __delay_cycles(DELAY);
    }

    return 0;
}
