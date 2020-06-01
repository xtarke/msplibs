/*
 * 10_main_spi_ds3234.c
 *
 *  Created on: Jun 1, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Exemplo de recepção e transmissão SPI com o RTC DS2132
 *      - Exemplo validado no Proteus
 *      - CPU é desligado até o recebimento dos dados.
 *      - Dados recebidos do RTC são enviados pela UART.
 *      - UART em 9600bps, 8bits de dados, sem paridade, 1 bit parada
 *
 *      - Clock da CPU é 16MHZ.
 *

 *               MSP430F2132
 *            -----------------
 *        /|\|            P1.0 |-> Slave Chip Select (GPIO)
 *         | |                 |
 *         --|RST         P3.4 |-> --> TX
 *           |                 |
 *           |            P3.4 |-> Data Out (UCA0SIMO)
 *           |                 |
 *           |            P3.5 |<- Data In (UCA0SOMI)
 *           |                 |
 *           |            P3.0 |-> Serial Clock Out (UCA0CLK)
 */


/* System includes */
#include "uart_spi_master_f2132.h"
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/bits.h"

#ifndef __MSP430F2132__
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
    uint8_t rtc_rx_data[8] = {0};
    uint8_t rtc_tx_data[5] = {0x00, 0x01, 0x02, 0x03, 0x05};

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= BIT2;
    SET_BIT(P1OUT,BIT2);

    /* Inicializa hardware */
    init_clock_system();
    init_uart();
    init_spi_master_mode();

    while(1){
        /* Envia e recebe os pacotes na mesma chamada.
         * SPI é Full Duplex */
        spi_master_send_receive_package(rtc_tx_data, rtc_rx_data, 5);
        /* Envia os dados recebidos pela UART */
        uart_send_package(rtc_rx_data, 4);
        _delay_cycles(10000000);
        CPL_BIT(P1OUT,BIT2);
    }

    /* Desliga a CPU  */
    __bis_SR_register(LPM0_bits + GIE);

    return 0;


}
