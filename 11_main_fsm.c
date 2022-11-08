/*
 * 11_main_fsm.c
 *
 *  Created on: Nov 7, 2022
 *      Author: Renan Augusto Starke
 *
 *      Um semáforo para exemplificar a implementação de FSM.
 *      WatchDog timer é utilizado como a base de tempo de 1 segundo
 *
 *
 *                  MSP430F5529LP
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *  BOTAO   --> | P1.1    P1.0    | --> LED VERMELHO
 *              |         P4.7    | --> LED VERDE
 *              |                 | --> Estado amarelo liga Vermelho e verde
 *
 */


#include <msp430.h>
#include <stdint.h>

#include "11_semaforo_fsm.h"

/**
  * @brief  Configura temporizador watchdog.
  *
  * @param  none
  *
  * @retval none
  */
void config_wd_as_timer(){
    /* Configura Watch dog como temporizador */

    WDTCTL = WDT_ADLY_1000;
    /* Ativa IRQ do Watchdog */
    SFRIE1 |= WDTIE;
}


int main(void)
{
    /* Desabilita Watch dog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Inicializa o hardware */
    config_wd_as_timer();
    config_semaforo();

    /* Entra em modo de economia de energia */
    __bis_SR_register(GIE);


    while (1) {
        do_fsm();

        /* Entra-se no modo de economia de energia.
         * Aguardando próximo wake-up          */
        __bis_SR_register(LPM0_bits);
    }


    return 0;
}





