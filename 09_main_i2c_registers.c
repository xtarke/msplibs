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
 *               DS1307    4k7 4k7     MSP430G2xx3
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->|P1.7/UCB0SDA       |-
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P1.6/UCB0SCL   P1.0| --> LED
 *              -------            |                   |
 *
 */

/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/i2c_master_g2553.h"
#include "lib/bits.h"

/* Define endereço de 7-bits I2C
 * 0x68 é o endereço do DS1307
 */
#define DEVICE_I2C_ADDR 0x68

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

int main(void) {

    uint8_t rtc_rx_data[8] = {0};
    uint8_t rtc_tx_data[4] = {0x07, 0x02, 0x03, 0x04};

    i2c_mode i2c_status = IDLE_MODE;

    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

    init_clock_system();
    init_i2c_master_mode();

    /* Debug LED */
    P1DIR |= BIT0;
    CLR_BIT(P1OUT,BIT0);

    /* Configura RTC escrevendo 4 valores a partir do endereço 0 */
    i2c_status = i2c_master_write_reg(DEVICE_I2C_ADDR, 0x00, rtc_tx_data, 4);

    if (i2c_status == NACK_MODE)
        SET_BIT(P1OUT, BIT0);

    _delay_cycles(10000000);

    /* Faz a leitura do RTC: */
    i2c_status = i2c_master_read_reg(DEVICE_I2C_ADDR, 0x00, 4, rtc_rx_data);
    if (i2c_status == NACK_MODE)
        SET_BIT(P1OUT, BIT0);

    __bis_SR_register(LPM0_bits + GIE);
    return 0;
}
