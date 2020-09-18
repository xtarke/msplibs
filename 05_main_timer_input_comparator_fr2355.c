/*
 * 05_main_timer_input_comparator.c
 *
 *  Created on: Apr 22, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de parcial de aplicação:
 *
 *      - Utiliza o evento de captura do Timer para borda
 *      de subida e descida de uma porta.
 *
 *                MSP430FR2355
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |       P2.0/TB1.1| <-- Sensor/botão
 *           |                 |
 *           |       P2.1/TB1.2| <-- Sensor/botão
 *           |                 |
 */

#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include <lib/bits.h>


#ifndef __MSP430FR2355__
    #error "Example not validated with this device."
#endif

/* Pinos de hardware */
#define SENSOR_PORT P2
#define SENSOR_PIN BIT0

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


/**
 * @brief  Configura temporizador A.
 *
 * @param  none
 *
 * @retval none
 */
void config_timerB_1(){
    /* Configura comparador 1 do timer B:
     * CM_3: captura de borda de subida e descida
     * CCIS_0: entrada A
     * CCIE: ativa IRQ
     * CAP: modo captura
     * SCS: captura síncrona
     */
    TB1CCTL1 |= CM_3 | CCIS_0 | CCIE | CAP | SCS;

    /* Configura timerB1:
     * - TBSSEL_2: SMCLK como clock source
     * - MC_2: modo de contagem contínua
     * - TBCLR: limpa registrador de contagem
     */
    TB1CTL |= TBSSEL_2 | MC_2 | TBCLR;
}


void main(void)
{
    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();
    config_timerB_1();

    /* LED de depuração */
    P1DIR = 1;

    /* Pull ups */
    P2REN = BIT0 | BIT1;
    P2OUT = BIT0 | BIT1;

    /* Input capture for P2.0 */
    P2SEL0 = 0x1;
    /* Input capture for P2.1 */
    P2SEL1 = 0x1;

    __bis_SR_register(GIE);

    while(1)
    {
        CPL_BIT(P1OUT,BIT0);

        __delay_cycles(1000000);
    }
}


// Timer1 Interrupt Handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) TIMER1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uint16_t timer_count_0 = 0;
    uint16_t timer_count_1 = 0;

    TB1CCTL0 &= ~CCIFG;


    /* ToDo: validar P2IN para detecção da borda */

    /* Borda de subida */
    if (TST_BIT(P2IN, BIT0)){
        timer_count_0 = TB1CCR1;
    }
    /* Borda de descida */
    else {
        timer_count_1 = TB1CCR1;

        /* Acorda main
        __bic_SR_register_on_exit(LPM0_bits); */
    }
}


