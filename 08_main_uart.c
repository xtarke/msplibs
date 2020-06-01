/*
 * 08_main_uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Exemplo de recepção e transmissão da USART
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
    const char message[] = "ACK";
    const char message_bin_data[] = { 65, 63, 87, 87};

    char my_data[8];

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Inicializa hardware */
    init_clock_system();
    init_uart();

    /* Led de depuração */
    P1DIR |= BIT0;

    while (1){
        /* Configura o recebimento de um pacote de 4 bytes */
        uart_receive_package((uint8_t *)my_data, 4);

        /* Desliga a CPU enquanto pacote não chega */
        __bis_SR_register(CPUOFF | GIE);

        /* Envia resposta */
        uart_send_package((uint8_t *)message,sizeof(message));

        /* Pisca LED para sinalizar que dados chegaram */
        CPL_BIT(P1OUT,BIT0);
    }
}
