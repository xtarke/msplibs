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

    /* Todos pinos como entrada */
    P1DIR = 0;
    /* Todos pino com pull up */
    P1REN = 0xff;

    /* Todos pinos como saída */
    P3DIR = 0xff;

    /* main não pode retornar */
    while(1){

        /*  Copia estado das entradas para porta de saída */
        P3OUT = P1IN;
    }

    return 0;
}
