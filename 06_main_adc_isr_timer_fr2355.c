/*
 * 06_main_adc_isr_timer_fr2355.c
 *
 *  Created on: Oct 02, 2020
 *      Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo do conversor analógico digital.
 *      - Trigger do ADC pelo timer B1.1
 *      - Sequẽncia de canais A0->A1->A2
 *      - ISR do timer desnecessário pois usa-se o hardware
 *      do timer para iniciar uma nova conversão
 *
 *
 *                  MSP430FR2355
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *  LED    <--  | P1.6    P1.0 A0 | <--
 *              |         P1.0 A1 | <--
 *              |         P1.0 A2 | <--
 *              |                 |
 *
 */

#include <msp430.h>
/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>
#include <stdio.h>

#include "lib/gpio.h"
#include "lib/bits.h"
#include "displays/lcd.h"

#ifndef __MSP430FR2355__
#error "Clock system not supported for this device"
#endif

volatile uint16_t adc_data[3] = {0};

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
  * @brief  Configura temporizador B1 para trigger do ADC
  *
  * @param  none
  *
  * @retval none
  */
void timerB_init(){

    /* Contagem máxima do timer 1 */
    TB1CCR0 = 200-1;
    /* Habilita saída interna do do comparador 0: CCR1 reset/set */
    TB1CCTL1 = OUTMOD_7;
    /* Valor de comparação 1: deve ser menor que TB1CCR0 */
    TB1CCR1 = 100;
    /* Configura timerB1:
    * - TBSSEL_2: SMCLK como clock source
    * - MC_2: modo de contagem contínua
    * - TBCLR: limpa registrador de contagem
    */
    TB1CTL = TBSSEL_2 | MC_2 | TBCLR;

}

void init_adc(){

    /* Configura pinos P1.0 a P.2 como entrada do AD */
    P1SEL0 |=  BIT0 + BIT1 + BIT2;
    P1SEL1 |=  BIT0 + BIT1 + BIT2;

    /* 16ADCclks, ADC ON */
    ADCCTL0 |= ADCSHT_2 | ADCON;
    /* ADC clock MODCLK, sampling timer, TB1.1B trig.,repeat sequence */
    ADCCTL1 |= ADCSHP | ADCSHS_2 | ADCCONSEQ_3;
    /* 8-bit conversion results */
    ADCCTL2 &= ~ADCRES;
    /* A0~2(EoS); Vref=1.5V */
    ADCMCTL0 |= ADCINCH_2 | ADCSREF_1;
    /* Enable ADC ISQ */
    ADCIE |= ADCIE0;

    /* Configure reference interna  */
    PMMCTL0_H = PMMPW_H;                                        // Unlock the PMM registers
   PMMCTL2 |= INTREFEN;                                        // Enable internal reference
   __delay_cycles(400);                                        // Delay for reference settling

    /* Enable ADC */
    ADCCTL0 |= ADCENC;
    /* Limpar timer para maior sincronismo */
    /* TB1CTL |= TBCLR;  */

}

int main (void)
{
    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;

    /* Debug LED */
    P6DIR |= BIT6;

    /* Sistema de clock:
     * cuidado em ligar 24MHZ no ADC.
     * Usar prescaller o MODCLOCK */
    //init_clock_system();
    timerB_init();
    init_adc();


    while (1){
        /* Desliga CPU até ADC terminar */
        __bis_SR_register(CPUOFF + GIE);
    }
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    static uint8_t i = 0;

    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:

            /* Obter amostras */
            adc_data[i] = ADCMEM0;

            if(i == 0)
                i = 2;
            else
                i--;

            P6OUT ^= BIT6;
            break;
        default:
            break;
    }
}


/* Timer1 Interrupt Handler */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) TIMER1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{


}

/* Timer0_B0 interrupt service routine
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    /* Caso trigger do ADC não funcione pelo TB1.1 */
    while(ADCCTL1 & ADCBUSY);
    /* Sampling and conversion start */
    ADCCTL0 |= ADCENC | ADCSC;

}

