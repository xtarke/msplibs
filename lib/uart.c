/*
 * uart.c
 *
 *  Created on: May 25, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
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

/* Project includes */
#include "uart.h"

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
#else
    #error "Clock system defined for UART support"
#endif

    /* Tipo de modulação e reset do hardware USCI */
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;

    /* Habilitação da ISR */
    IE2 |= UCA0RXIE;

}

void uart_send_data(char data){
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;                    // TX -> RXed character
}




/* ISR de recebimento (RX) da UART */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}


/* ISR de envio (TX) da UART */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  // while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  // UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}

