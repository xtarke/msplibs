/*
 * 01_main_simple_gpio.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#include "lib/bits.h"

#define LED_1   BIT0
#define LED_2   BIT6
#define BUTTON  BIT3

/**
  * @brief  Configura hardware.
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
void hardware_init()
{
    /* Acesso direto: P1.0 e P1.6 como saídas. Demais como entrada */
    P1DIR |= LED_1 | LED_2;

    /* Habilita resistor de pull up ou pull down */
    P1REN |= BUTTON;
    /* Habilita resitor como pull up */
    P1OUT |= BUTTON;
}

/**
  * @brief  main: todo programa C deve ter!
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
int main(void)
{
    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;

    /* Incializa o hardware */
    hardware_init();

    /* Laço infinito */
    while(1){

        if (TST_BIT(P1IN, BUTTON)) /* Equivalente a: if (P1IN & BIT3) */
            CLR_BIT(P1OUT, LED_1 | LED_2);    /* Equivalente a P1OUT &= ~(BIT0 | BIT6); */
        else
            SET_BIT(P1OUT, LED_1 | LED_2);    /* Equivalente a: P1OUT |= BIT0 | BIT6; */
    }

    return 0;
}
