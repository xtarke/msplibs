/*
 * 09_main_i2c_scan.c
 *
 *  Created on: May 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Aplicação que escreve e lê dados do CI I2C DS1307.
 *
 *      - Se usado Analizador "Logic", lembrar que o endereço
 *      deve ser divido por 2, >> 1, pois são 7 bits.
 *
 *                          .   .
 *                         /|\ /|\
 *               DS1307    4k7 4k7     MSP430FR2355
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->|P1.2/UCB0SDA       |-
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P1.3/UCB0SCL   P1.0| --> LED
 *              -------            |                   |
 *
 */

/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/i2c_master_fr2355.h"
#include "lib/bits.h"

/* Define endereço de 7-bits I2C
 * 0x68 é o endereço do DS1307
 */
#define DEVICE_I2C_ADDR 0x68

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
int main(void) {

    uint8_t rtc_rx_data[8] = {0};
    uint8_t rtc_tx_data[4] = {0x07, 0x02, 0x03, 0x04};

    i2c_mode i2c_status = IDLE_MODE;

    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();
    init_i2c_master_mode();

    /* Debug LED */
    P1DIR |= BIT0;
    CLR_BIT(P1OUT,BIT0);

    /* Configura RTC escrevendo 4 valores a partir do endereço 0 */
    i2c_status = i2c_master_write_reg(DEVICE_I2C_ADDR, 0x00, rtc_tx_data, 4);

    if (i2c_status == NACK_MODE)
        SET_BIT(P1OUT, BIT0);

    _delay_cycles(10000);

    /* Faz a leitura do RTC: */
    i2c_status = i2c_master_read_reg(DEVICE_I2C_ADDR, 0x00, 4, rtc_rx_data);
    if (i2c_status == NACK_MODE)
        SET_BIT(P1OUT, BIT0);

    __bis_SR_register(LPM0_bits + GIE);
    return 0;
}
