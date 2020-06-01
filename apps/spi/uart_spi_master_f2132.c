/*
 * uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Biblioteca de comunicação SPI e USART
 *      - Baseado na documentação msp430g2xx3_usci_spi_standard_master.c
 *      de Nima Eskandari de Texas Instruments Inc.
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
#include <apps/spi/uart_spi_master_f2132.h>
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "../../lib/bits.h"
#include "../../lib/gpio.h"

#ifndef __MSP430F2132__
#error "Library no supported/validated in this device."
#endif

typedef enum spi_state_enum {
    SPI_IDLE,
    SPI_TX_DATA,
    SPI_RX_DATA,
    SPI_RX_TX_DATA
} spi_state_t;

struct spi_status_t {
    spi_state_t fsm;

    /* Estado de envio */
    uint8_t *data_to_send;
    uint8_t send_size;
    uint8_t send_busy;

    /* Estado de recepção */
    uint8_t *data_to_receive;
    uint8_t receive_size;
    uint8_t receive_busy;
};

volatile struct spi_status_t spi_status = {0};


struct uart_status_t {
    /* Estado de envio */
    uint8_t *data_to_send;
    uint8_t send_size;
    uint8_t send_busy;

    /* Estado de recepção */
    uint8_t *data_to_receive;
    uint8_t receive_size;
    uint8_t receive_busy;
};

volatile struct uart_status_t uart_status = {0};

/**
 * @brief  Configura o hardware USCI0 para UART com baudrate em 9600.
 *         DCO em 1 MHz. Ajustar
 *
 * @param  none
 *
 * @retval none
 */
void init_spi_master_mode(){
    /* Função alternativa dos pinos:
     * - P3.5 = UCA0SOMI (Slave Output, Master Input) -- MISO
     * - P3.4 = UCA0SIMO (Slave Input, Master Output) -- MOSI
     * - P3.0 = UCA0CLK -- Serial Clock Out
     */
    P3SEL |= BIT1 | BIT2 | BIT3;

    /* Slave select */
    SET_BIT(PORT_DIR(SLAVE_CS_PORT), SLAVE_CS_PIN);
    SET_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);

    /* Clock Polarity: The inactive state is high *
     * MSB First, 8-bit, Master, 3-pin mode, Synchronous */
    UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;
    /* SMCLOCK */
    UCB0CTL1 |= UCSSEL_2;

    /* Bit rate */
#ifdef CLOCK_16MHz
    UCB0BR0 |= 0x20;
    UCB0BR1 = 0;
    // UCB0MCTL = 0;
#else
#error "Clock system not defined for SPI support"
#endif

    /* Start hardware */
    UCB0CTL1 &= ~UCSWRST;
    /* Reset dirty flags */
    IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);

    /* Habilitação da ISR */
    IE2 |= UCB0RXIE;
}

/**
 * @brief  Configura o hardware USCI0 para UART com baudrate em 9600.
 *         DCO em 1 MHz. Ajustar
 *
 * @param  none
 *
 * @retval none
 */
void init_uart(){
    /* Função alternativa dos pinos:
     * - P3.4 = TXD
     * - P3.5 = RXD
     */
    P3SEL |= BIT4 | BIT5;

#ifdef CLOCK_1MHz
    /* Fonte de clock da UART: SMCLK */
    UCA0CTL1 |= UCSSEL_2;
    /* Configuração de clock: ver página 424 do MSP430G2553 Family Guide */
    UCA0BR0 = 104;
    UCA0BR1 = 0;
#elif defined CLOCK_16MHz
    /* Fonte de clock da UART: SMCLK */
    UCA0CTL1 |= UCSSEL_2;
    /* Configuração de clock: ver página 424 do MSP430G2553 Family Guide */
    UCA0BR0 = 0x82;
    UCA0BR1 = 0x06;
#else
#error "Clock system not defined for UART support"
#endif

    /* Tipo de modulação e reset do hardware USCI */
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;

    /* Reset dirty flags */
    IFG2 &= ~(UCA0TXIFG | UCA0RXIFG);

    /* Habilitação da ISR */
    IE2 |= UCA0TXIE;
}

/**
 * @brief  Envia um pacote para o USCI.
 *         Utiliza IRQ de transmissão para o envio dos bytes.
 *
 *         Use com IRS habilitadas.
 *
 * @param  data: endereço inicial dos dados do pacote.
 *         size: tamanho do pacote.
 *
 * @retval none
 */
void uart_send_package(uint8_t *data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (uart_status.send_size != 0);

    /* Condições de estado */
    uart_status.data_to_send = data;
    uart_status.send_size = size - 1;
    uart_status.send_busy = 1;

    /* Envia primeiro byte */
    UCA0TXBUF = data[0];

    __bis_SR_register(CPUOFF + GIE);
}

/**
 * @brief  Envia um pacote para o USCI.
 *         Utiliza IRQ de transmissão para o envio dos bytes.
 *
 *         Use com IRS habilitadas.
 *
 * @param  data: endereço inicial dos dados do pacote.
 *         size: tamanho do pacote.
 *
 * @retval none
 */
void spi_master_send_package(uint8_t *data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (spi_status.send_size != 0);

    /* Condições de estado */
    spi_status.data_to_send = data;
    spi_status.send_size = size - 1;
    spi_status.send_busy = 1;
    spi_status.fsm = SPI_TX_DATA;

    CLR_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);

    /* Envia primeiro byte */
    UCB0TXBUF = data[0];

    __bis_SR_register(CPUOFF + GIE);

}


/**
 * @brief  Envia um pacote para o USCI.
 *         Utiliza IRQ de transmissão para o envio dos bytes.
 *
 *         Use com IRS habilitadas.
 *
 * @param  data: endereço inicial dos dados do pacote.
 *         size: tamanho do pacote.
 *
 * @retval none
 */
void spi_master_send_receive_package(uint8_t *tx_data, uint8_t *rx_data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (spi_status.send_size != 0);

    /* Condições de estado */
    spi_status.data_to_send = tx_data;
    spi_status.data_to_receive = rx_data;
    spi_status.send_size = size - 1;
    spi_status.send_busy = 1;
    spi_status.fsm = SPI_RX_TX_DATA;

    CLR_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);

    /* Envia primeiro byte */
    UCB0TXBUF = tx_data[0];

    __bis_SR_register(CPUOFF + GIE);

}


void spi_master_receive_package(uint8_t *data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (spi_status.receive_size != 0);

    /* Condições de estado */
    spi_status.data_to_receive = data;
    spi_status.receive_size = size;
    spi_status.receive_busy = 1;
    spi_status.fsm = SPI_RX_DATA;

    CLR_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);

    /* Envia primeiro byte */
    UCB0TXBUF = 0xff;

    __bis_SR_register(CPUOFF + GIE);
}


/* ISR de envio (TX) da UART. Acontece quando o buffer de transmissão está vazio */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    /* USCIxTX ISR é compartilhado entre o módulo A e B
     *
     * Verifica se é uma IRQ do módulo A: */

    if (IFG2 & UCB0RXIFG){

        uint8_t uca0_rx_val = UCB0RXBUF;

        switch (spi_status.fsm){
            case SPI_IDLE:
                break;

            case  SPI_TX_DATA:
                if (spi_status.send_size != 0){
                    UCB0TXBUF = *(++spi_status.data_to_send);
                    spi_status.send_size--;
                }
                else {
                    spi_status.fsm = SPI_IDLE;
                    /* Condições de término de envio de pacote */
                    /* Desbloqueia serialização de pacotes */
                    spi_status.send_busy = 0;
                    /* Limpa flag de final de transmissão pois não há
                     * mais nada a enviar          */
                    IFG2 &= ~(UCB0TXIFG);
                    SET_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);
                    __bic_SR_register_on_exit(CPUOFF);
                }
                break;

            case SPI_RX_DATA:
                if (spi_status.receive_size)
                {
                    *(spi_status.data_to_receive++) = uca0_rx_val;
                    spi_status.receive_size--;
                }

                if (spi_status.receive_size == 0)
                {
                    spi_status.fsm = SPI_IDLE;
                    spi_status.receive_busy = 0;
                    SET_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);
                    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
                }
                else {
                    /* Envia algo para gerar clock */
                    UCB0TXBUF = 0xff;
                }
                break;

            case SPI_RX_TX_DATA:
                if (spi_status.send_size != 0){
                    UCB0TXBUF = *(++spi_status.data_to_send);
                    spi_status.send_size--;

                    *(spi_status.data_to_receive++) = uca0_rx_val;

                }
                else {
                    spi_status.fsm = SPI_IDLE;
                    /* Condições de término de envio de pacote */
                    /* Desbloqueia serialização de pacotes */
                    spi_status.send_busy = 0;
                    /* Limpa flag de final de transmissão pois não há
                     * mais nada a enviar          */
                    IFG2 &= ~(UCB0TXIFG);
                    SET_BIT(PORT_OUT(SLAVE_CS_PORT), SLAVE_CS_PIN);
                    __bic_SR_register_on_exit(CPUOFF);
                }
                break;

            default:
                break;
        }
    }
    else{

        /* Código do módulo B */

    }
}


/* ISR de envio (TX) da UART. Acontece quando o buffer de transmissão está vazio */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{

    /* USCIxTX ISR é compartilhado entre o módulo A e B
     *
     * Verifica se é uma IRQ do módulo A: */
    if (IFG2 & UCA0TXIFG){
        if (uart_status.send_size != 0){
            UCA0TXBUF = *(++uart_status.data_to_send);
            uart_status.send_size--;
        }
        else {
            /* Condições de término de envio de pacote */
            /* Desbloqueia serialização de pacotes */
            uart_status.send_busy = 0;
            /* Limpa flag de final de transmissão pois não há
             * mais nada a enviar          */
            IFG2 &= ~(UCA0TXIFG);
            __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        }

    }
    else{
        //IFG2 &= ~(UCB0TXIFG);
        /* Código do módulo B */


    }
}
