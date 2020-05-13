/*
 * main_fsm.c
 *
 *  Created on: May 13, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de parcial de aplicação:
 *
 *      - PWM em 50 Hz para controle de servo motores.
 *      - Servo motor 1ms -> eixo em 0º,  2ms eixo em 180º
 *      - Máquina de estado (FSM) para o controle de um processo discreto.
 *
 *
 *               MSP430G2553
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |             P1.0| --> LED vermelho: processo ativo
 *           |             P1.2| --> Servo motor (PWM @ 50Hz)
 *           |             P1.3| <-- Botão START do processo
 *           |
 */

/* Includes de sistema */
#include <msp430.h>

/* Includes do projeto */
#include "../../lib/gpio.h"
#include "../../lib/bits.h"
#include "control.h"

#ifndef __MSP430G2553__
    #error "Clock system not supported for this device"
#endif

void init_clock_system(){

    /* Configuração do MCLK em 8MHz */

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

void main(){
    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Systema de clock e applicação */
    init_clock_system();
    control_init();

    __bis_SR_register(LPM0_bits + GIE);

    while (1){
        do_control();
        __bis_SR_register(LPM0_bits);
    }
}


