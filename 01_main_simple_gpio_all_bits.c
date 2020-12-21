/*
 * 01_main_simple_blink.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#define DELAY 10000

int main(void)
{
    int16_t i;

    /* Desabilita Watch dog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    P1DIR = 0xff;
    P2DIR = 0xff;
    P3DIR = 0xff;

    /* main não pode retornar */
    while(1){
        /* Liga/Desliga LED */
        P1OUT = P1OUT ^ 0xff;
        P2OUT = P2OUT ^ 0xff;
        P3OUT = P3OUT ^ 0xff;

        /* Atraso */
        for (i=DELAY; i > 0; i--);
    }

    return 0;
}
