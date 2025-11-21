/*
 * 09_main_i2c_scan.c
 *
 *  Created on: 21/11/2025
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Leitura de Temperatura usando o Sensor IR GY-90615
 *      - Dados enviados pela USART
 *
 *
 *
 *                          .   .
 *                         /|\ /|\
 *              GY-90615    4k7 4k7     MSP430FR2355
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
#include <stdio.h>
#include <string.h>
#include <msp430.h>

/* Project includes */
#include "lib/i2c_master_fr2355.h"
#include "lib/bits.h"
#include "lib/uart_fr2355.h"

/* Endereço do Sensor IR GY-90615 */
#define DEVICE_I2C_ADDR 0x5b

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

void config_wd_as_timer(){
    /* Configura Watch dog como temporizador:
     *
     * WDT_ADLY_1000 <= (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)
     *
     * WDTPW -> "Senha" para alterar confgiuração.
     * WDTTMSEL -> Temporizador ao invés de reset.
     * WDTSSEL -> Fonte de clock de ACLK
     * WDTIS2 -> Período de 1000ms com ACLK = 32.768Hz
     *
     */
    WDTCTL = WDT_ADLY_1000;
    /* Ativa IRQ do Watchdog */
    SFRIE1 |= WDTIE;
}


int main(void) {
    char message[32] = {0};
    i2c_mode i2c_status = IDLE_MODE;
    uint8_t data[4];
    uint16_t rawData;
    uint32_t temperature;


    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();
    init_i2c_master_mode();
    init_uart();
    config_wd_as_timer();

    /* Debug LED */
    P1DIR |= BIT0;
    CLR_BIT(P1OUT,BIT0);

    __bis_SR_register(GIE);

    while(1){

        /* Lê registrador 0x07 e comando 0010 para acesso da ram */
        i2c_status = i2c_status = i2c_master_read_reg(DEVICE_I2C_ADDR, 0x27, 3, data);

        /* Cálculo da temperatura: ver datasheet do sensor */
        rawData = data[1] << 8 | data[0];
        /* T [°C] = RAM (0x07) × 02.0 − 273 15 */
        temperature = (int32_t)(rawData * 2) - 27315;

        snprintf(message, sizeof(message) , "T: %d C\r\n", (int16_t)(temperature));

        uart_send_package((uint8_t *)message,strlen(message),0);

        /* Pisca LED para sinalizar que dados estão sendo enviados */
        CPL_BIT(P1OUT,BIT0);

        __bis_SR_register(CPUOFF | GIE);

    }
    return 0;
}


/* ISR do watchdog: executado toda a vez que o temporizador estoura */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(CPUOFF);
}


