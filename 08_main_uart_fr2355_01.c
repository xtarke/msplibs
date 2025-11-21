/*
 * 08_main_uart.c
 *
 *  Created on: Jun 4, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      - Exemplo de recepção e transmissão da USART
 *      - CPU é desligado até o recebimento dos dados.
 *      - Uma mensagem de ACK é enviado quando um pacote
 *      é recebido.
 *
 *      - Clock da CPU é 24MHZ definido e uart_fr2355.h  devido a
 *      configuração do baudrate.
 *
 *      - VEJA uart_fr2355.c/.h
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
#include <lib/uart_fr2355.h>
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/bits.h"

#ifndef __MSP430FR2355__
#error "Clock system not supported/tested for this device"
#endif


/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO) em 24MHz
  *         Essa configuração utiliza pinos para cristal externo.
  * @param  none
  *
  * @retval none
  */
void init_clock_system(void) {

    // Configure two FRAM wait state as required by the device data sheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 = DCORSEL_7;                          // Set DCO = 24MHz
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 327358*731 / 1
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    /* CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
     * set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
     * default DCOCLKDIV as MCLK and SMCLK source
     - Selects the ACLK source.
     * 00b = XT1CLK with divider (must be no more than 40 kHz)
     * 01b = REFO (internal 32-kHz clock source)
     * 10b = VLO (internal 10-kHz clock source) (1)   */
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
}


int main(){
    const char message[] = "ACK";
    const char message_bin_data[] = { 65, 63, 87, 87};

    char my_data[8];

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;

    /* Inicializa hardware */
    init_clock_system();
    init_uart();

    /* Led de depuração */
    P1DIR |= BIT0;

    __bis_SR_register(GIE);

    while (1){
        /* Configura o recebimento de um pacote de 4 bytes */
        uart_receive_package((uint8_t *)my_data, 4);

        /* Desliga a CPU enquanto pacote não chega */
        __bis_SR_register(CPUOFF | GIE);

        /* Envia resposta, CPU é ACORDADA quando size bytes forem enviados */
        uart_send_package((uint8_t *)message, 4, 1);

        /* Pisca LED para sinalizar que dados chegaram */
        CPL_BIT(P1OUT,BIT0);

        __bis_SR_register(CPUOFF | GIE);
    }
}
