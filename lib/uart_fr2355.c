/*
 * uart.c
 *
 *  Created on: Jun 4, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Biblioteca de comunicação UART.
 *      - Utiliza UCA1 em modo UART 115200 bps
 *
 *               MSP430FR2355
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |    P4.3/UCA1TXD | --> TX
 *           |                 |
 *           |    P4.2/UCA1RXD | <-- RX
 *           |                 |
 */

/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include <lib/uart_fr2355.h>

#ifndef __MSP430FR2355__
#error "Library no supported/validated in this device."
#endif

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
 * @brief  Configura o hardware USCI0 para UART com baudrate em 115200.
 *         FLL em 24 MHz. Ajustar o baudrate conforme a frequência de
 *         operação da CPU
 *
 * @param  none
 *
 * @retval none
 */
void init_uart(){
    /* Função alternativa dos pinos:
     * - P4.2 = RXD
     * - P4.3 = TXD
     */
    P4SEL0 = BIT2 | BIT3;

#ifdef CLOCK_24MHz
    UCA1CTLW0 |= UCSWRST;
    /* Fonte de clock SMCLK */
    UCA1CTLW0 |= UCSSEL_2;

    /* Veja http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    UCA1BR0 = 13;
    UCA1BR1 = 0;
    UCA1MCTLW = 0x2500 | UCOS16;
#else
#error "Clock system not defined for UART support"
#endif

    /* Initicialização do eUSCI */
    UCA1CTLW0 &= ~UCSWRST;

    /* Habilitação da ISR */
    UCA1IE |= UCRXIE | UCTXIE;
}


/**
 * @brief  Envia um pacote para o ESCI.
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
    while (uart_status.send_busy != 0);

    /* Condições de estado */
    uart_status.data_to_send = data;
    uart_status.send_size = size - 1;
    uart_status.send_busy = 1;

    /* Envia primeiro byte */
    UCA1TXBUF = data[0];
}

/**
 * @brief  Recebe um pacote do ESCI.
 *         Utiliza IRQ de recebimento de cada byte.
 *
 *         Use com IRS habilitadas.
 *
 * @param  data: endereço inicial dos dados do pacote.
 *         size: tamanho do pacote.
 *
 * @retval none
 */
void uart_receive_package(uint8_t *data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (uart_status.receive_busy != 0);

    /* Condições de estado */
    uart_status.data_to_receive = data;
    uart_status.receive_size = size;
    uart_status.receive_busy = 1;
}


/* ISR da EUSCI: acontece quando algum byte é recebido, transmitido, etc
 * conforme habilitações na inicialização  */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uint8_t data;

    switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE:
            break;
        case USCI_UART_UCRXIFG:     /* Received IRQ */
            data = UCA1RXBUF;

            if (uart_status.receive_busy)
            {
                /* Guarda dados */
                *(uart_status.data_to_receive++) = data;
                uart_status.receive_size--;

                /* Se todos os dados foram recebidos,
                 * acorda main e deslia recepção */
                if (uart_status.receive_size == 0){
                    uart_status.receive_busy = 0;
                    __bic_SR_register_on_exit(CPUOFF);
                }
            }

        case USCI_UART_UCTXIFG:     /* Transmit IRQ */
            if (uart_status.send_size != 0){
                UCA1TXBUF = *(++uart_status.data_to_send);
                uart_status.send_size--;
            }
            else {
                /* Condições de término de envio de pacote */
                /* Desbloqueia serialização de pacotes */
                uart_status.send_busy = 0;
                /* Limpa flag de final de transmissão pois não há
                 * mais nada a enviar          */
                __bic_SR_register_on_exit(CPUOFF);
            }


        case USCI_UART_UCSTTIFG:    /*  START byte received interrupt. */
            break;
        case USCI_UART_UCTXCPTIFG:  /* Transmit complete interrupt. */
            break;

        default: break;
        }
}
