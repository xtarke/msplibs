/*
 * 01_main_simple_blink.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#define LED   BIT3
#define DELAY 5000

int main(void)
{
    int i;

    /* Desabilita Watch dog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    P2DIR = 0xff;

    ADC10AE0 = 0;
    CAPD = 0 ;

    /* main não pode retornar */
    while(1){
        /* Liga/Desliga LED */
        P2OUT = P2OUT ^ LED;
        P2OUT = P2OUT ^ BIT2;

        /* Atraso */
        for (i=DELAY;i--; i > 0);
    }

    return 0;
}
