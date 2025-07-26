/*
 * main.c
 *
 *  Created on: Jul 25, 2025
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - OLED SDD1366 example application
 *
 *           OLED SSD1306              MSP430G2553
 *             +-------+           +-------------------+
 *             |    SDA|<  -|---+->|P3.1/UCB0SDA       |        
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P3.2/UCB0SCL   P2.0|
 *              -------            |                   |
 *
 */

/* System includes */
#include <string.h>
#include <msp430.h>
#include <stdint.h>
#include "intrinsics.h"

/* Project includes */
#include "./lib/i2c_master_g2553.h"
#include "./lib/bits.h"
#include "./lib/gpio.h"
#include "displays/ssd1366.h"

#define LED BIT0
#define LED_PORT P1


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

    /* Configure ACLK as VLO: ~12KHz
     * LFXT1 = VLO */
    BCSCTL3 |= LFXT1S_2;

}

/**
  * @brief  Configura temporizador watchdog.
  *
  * @param  none
  *
  * @retval none
  */
void config_wd_as_timer(){
    /* Configura Watch dog como temporizador:
     *
     * WDT_ADLY_250 <= (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)
     *
     * WDTPW -> "Senha" para alterar confgiuração.
     * WDTTMSEL -> Temporizador ao invés de reset.
     * WDTSSEL -> Fonte de clock de ACLK
     * WDTIS1+WDTIS0 -> Clock / 8192
     *
     */
    WDTCTL = WDT_ADLY_1000;
    /* Ativa IRQ do Watchdog */
    IE1 |= WDTIE;
}


int main(void) {
    int i;
    /* Desliga Watchdog */
    WDTCTL = WDTPW | WDTHOLD;

    init_clock_system();
    init_i2c_master_mode();
    config_wd_as_timer();

    /* Debug LED */
    P1DIR |= BIT0;
    SET_BIT(P1OUT,BIT0);

    __bis_SR_register(LPM0_bits + GIE);
 
    ssd1306_init();

    ssd1306_draw_pixel(16,16,WHITE_PIXEL);
    ssd1306_write_scaled_char(0,0, 'R',2);
    ssd1306_write_scaled_char(16,0, ':',2);
    ssd1306_write_scaled_char(0,24, 'b',2);
    ssd1306_write_scaled_char(16,24, ':',2);

    ssd1306_display_data();

    while (1){

        ssd1306_display_data();

        __bis_SR_register(LPM0_bits + GIE);        
    }
    
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
    static uint16_t x = 0;

    PORT_OUT(LED_PORT) ^= LED;

    __bic_SR_register_on_exit(CPUOFF);

    if (x >= 10) {

        x = 0;
        //__bic_SR_register_on_exit(CPUOFF);
    }
    x++;
}
