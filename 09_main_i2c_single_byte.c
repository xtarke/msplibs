/*
 * 09_main_i2c_scan.c
 *
 *  Created on: May 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Escreve apenas um byte no barramento I2C.
 *      - Exemplo útil para expansor de IO PCF8574T.
 *                          .   .
 *                         /|\ /|\
 *               CI_xyz    10k 10k     MSP430G2xx3
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->|P1.7/UCB0SDA       |-
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P1.6/UCB0SCL       |
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
 * 0x27 é o endereço do PCF8574T -- NXP
 */
#define DEVICE_I2C_ADDR 0x27

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

    uint8_t i=0;
    i2c_mode i2c_status = IDLE_MODE;

    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

    init_clock_system();
    init_i2c_master_mode();

    /* Debug LED */
    P1DIR |= BIT0;
    CLR_BIT(P1OUT,BIT0);

    while (1){
        i2c_status = i2c_write_single_byte(DEVICE_I2C_ADDR, 0x00);
       _delay_cycles(1000000);

       if (i2c_status == NACK_MODE)
           SET_BIT(P1OUT,BIT0);

       i2c_status = i2c_write_single_byte(DEVICE_I2C_ADDR, 0xff);

       if (i2c_status == NACK_MODE)
            SET_BIT(P1OUT,BIT0);

       _delay_cycles(1000000);
    }

    __bis_SR_register(LPM0_bits + GIE);
    return 0;
}


