/*
 * 06_main_analog_comp_pooling.c
 *
 *  Created on: May 15, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo do comparador analógico em modo pooling.
 *
 *               MSP430G2553
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |             P1.0| --> LED vermelho
 *           |                 |
 *           |       P1.1 CA1  | <-- Potenciômetro
 *           |                 |
 */


#include <msp430.h>
/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include "lib/gpio.h"
#include "lib/bits.h"

#define LED_PORT P1
#define LED_PIN  BIT0


#ifndef __MSP430G2553__
#error "Clock system not supported for this device"
#endif


/**
 * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
 *         Utililiza-se as calibrações internas gravadas na flash.
 *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c
 *         Configura ACLK para utilizar VLO = ~10KHz
 * @param  none
 *
 * @retval none
 */
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



void config_analog_comparator(){

    /* CACTL1: Comparator A Control 1 *
     * CARSEL: Comp. A Internal Reference Enable
     * CAREF0: Comp. A Internal Reference Select 0 (0.5 Vcc)
     * CAON  : Comp. A enable
     *
     * P2CA4 : Comp. A +Terminal Multiplexer
     *
     *
                    |+
    CA1      ---(+)-|   +
                    |      +-------  CAOUT
    0.5 VCC --- (-) |   +
                    |+
     */

    /* Liga comparador, seleciona entrada negativa */
    CACTL1 = CARSEL + CAREF1 + CAON;
    /* Seleciona entrada positiva */
    CACTL2 = P2CA4;
}


int main (void)
{
    WDTCTL = WDTPW + WDTHOLD;

    /* Sistema de clock */
    init_clock_system();
    config_analog_comparator();

    /* Porta de saída */
    PORT_DIR(LED_PORT) = LED_PIN;

    while (1)
    {
        /* Pooling da saída do comparador */
        if ((CAOUT & CACTL2))
            SET_BIT(PORT_OUT(LED_PORT), LED_PIN);
        else
            CLR_BIT(PORT_OUT(LED_PORT), LED_PIN);
    }
}

