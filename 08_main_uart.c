/*
 * 08_main_uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 */

/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/uart.h"

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
/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
  *         Utililiza-se as calibrações internas gravadas na flash.
  *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c
  * @param  none
  *
  * @retval none
  */
void init_clock_system(){

#ifdef CLOCK_1MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_1MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#endif

#ifdef CLOCK_8MHz

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Outras fonte de clock devem ser configuradas também *
     * de acordo com a aplicação  */
#endif

#ifdef CLOCK_12MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_12MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#endif

#ifdef CLOCK_16MHz
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_16MHZ==0xFF)
        while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#endif

}


void main(){
    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    init_clock_system();
    init_uart();


    __bis_SR_register(GIE);

    while (1){
        uart_send_data('o');


        _delay_cycles(1000000);



    }


}
