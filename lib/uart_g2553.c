/*
 * uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Biblioteca de comunicação UART.
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
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include <lib/uart_g2553.h>

#ifndef __MSP430G2553__
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
  * @brief  Configura o hardware USCI0 para UART com baudrate em 9600.
  *         DCO em 1 MHz. Ajustar
  *
  * @param  none
  *
  * @retval none
  */
void init_uart(){
    /* Função alternativa dos pinos:
     * - P1.1 = RXD
     * - P1.2 = TXD
     */
    P1SEL = BIT1 | BIT2;
    P1SEL2 = BIT1 | BIT2;

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
    IE2 |= UCA0RXIE | UCA0TXIE;

}

/**
  * @brief  Envia um byte para o USCI.
  *         Modo pooling, usa apenas para teste.
  *
  *         NÂO use com ISRs habilitadas.
  *
  * @param  none
  *
  * @retval none
  */
void uart_send_data_pooling(uint8_t byte){
    /* Aguarda buffer de hardware se ocupado */
    while (!(IFG2&UCA0TXIFG));
    /* Copia dado para o buffer de hardware */
    UCA0TXBUF = byte;
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
}


void uart_receive_package(uint8_t *data, uint8_t size){

    /* Serializa a transmissão de vários pacotes */
    while (uart_status.receive_size != 0);

    /* Condições de estado */
    uart_status.data_to_receive = data;
    uart_status.receive_size = size;
    uart_status.receive_busy = 1;
}


/* ISR de recebimento (RX) da UART: acontece quando algum byte é recebido */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uint8_t data;

    /* USCIxRX ISR é compartilhado entre o módulo A e B
     *
     * Verifica se é uma IRQ do módulo A: */
    if (IFG2 & UCA0RXIFG){
        data = UCA0RXBUF;

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
    }
    else
    {
        /* Código do módulo B */
        data = UCB0RXBUF;
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
        }

    }
    else{

        /* Código do módulo B */


    }
}

