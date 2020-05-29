/*
 * 08_main_uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Exemplo de recepção e transmissão da USAR
 *      - CPU é desligado até o recebimento dos dados.
 *      - Uma mensagem de ACK é enviado quando um pacote
 *      é recebido.
 *
 *      - Clock da CPU é 1MHZ definido e uart.h  devido a
 *      configuração do baudrate.
 *
 *      - VEJA uart.c/.h
 *
 *               MSP430G2553
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |    P1.2/UCA0TXD | --> TX
 *           |                 |
 *           |    P1.1/UCA0RXD | <-- RX
 *           |                 |
 */


/* System includes */
#include <lib/uart_g2553.h>
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/bits.h"

#ifndef __MSP430G2553__
#error "Clock system not supported for this device"
#endif


/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
  *         Utililiza-se as calibrações internas gravadas na flash.
  *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c  *
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


int main(){
    uint8_t rtc_rx_data[4] = {0,0,0,0};
    uint8_t rtc_tx_data[4] = {0x07, 0x02, 0x03, 0x04};

    char my_data[8];

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Inicializa hardware */
    init_clock_system();
    init_spi_master_mode();

    /* Led de depuração */
    P1DIR |= BIT0;

    /* Configura o recebimento de um pacote de 4 bytes */
    spi_master_receive_package((uint8_t *)my_data, 4);

    spi_master_send_package(rtc_tx_data, 1);
    _delay_cycles(10000);

    spi_master_receive_package(rtc_rx_data, 4);

    /* Desliga a CPU  */
    __bis_SR_register(LPM0_bits + GIE);

    return 0;


}
