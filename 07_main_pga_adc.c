/*
 * 07_main_pga_adc.c
 *
 *  Created on: May 21, 2020
 *      Author: Renan Augusto Starke
 *
 *                MSP430FR235x
 *            -----------------
 *        /|\|              XIN|-
 *         | |                 |
 *         --|RST          XOUT|-
 *           |                 |
 *           |       P1.2/OA0- | <-- Entrada negativa (não ligado externamente pois está no modo não-inversor)
 *           |                 |
 *           |      P1.1/OA0O  | --> Saída do PGA (não ligado externamente pois ligamos a saída no ADC12)
 *           |                 |
 *           |      P1.3/OA0+  | <-- Entrada positiva (Sensor LM35 ou potenciômetro)
 */


#include <msp430.h>


#ifndef __MSP430FR2355__
#error "Clock system not supported for this device"
#endif

/**
  * @brief  Configura o Programmable Gain Amplifier (PGA).
  *
  * @param  none
  *
  * @retval none
  */
void init_pga(){
    /* SAC0OA: SAC OA Control Register *
     * NMUXEN: SAC Negative input MUX controL
     * PMUXEN: SAC Positive input MUX control
     * PSEL_0: External source selected
     * NSEL_1: PGA source selected
     * OAPM  : SAC OA power mode selection
     *
     * SAC0PGA: SAC PGA Control Register
     * GAIN0 :  SAC PGA Gain configuration: gain=1
     * MSEL_2: Non-inverting mode
     *
     *
                        |+
      P1.3/OA0+   --(+)-|   +
                        |      +-------  P1.1/OA0O    (Ligado no canal A1 do ADC internamente)
      P1.2/OA0- --- (-) |   +
                        |+
     */

    SAC0OA = NMUXEN + PMUXEN + PSEL_0 + NSEL_1;
    SAC0OA |= OAPM;
    SAC0PGA = GAIN0 + MSEL_2;
    SAC0OA |= SACEN + OAEN;

    /* Configuração da função alternativa dos pinos
     *
     * Nesse caso apenas configura-se a entrada positiva (P1.3/OA0+)
     * Demais sinais não estão conectados aos pinos externos.
     *
     * Se necessário, liga-se todos assim:
     *
     * P1SEL0 |= BIT1 | BIT2 | BIT3;
     * P1SEL1 |= BIT1 | BIT2 | BIT3
     */

    P1SEL0 |= BIT3;
    P1SEL1 |= BIT3;
}

/**
  * @brief  Configura conversor analógico digital (ADC12).
  *
  * @param  none
  *
  * @retval none
  */
void init_adc12(){

    /* ADCCTL0:
     * ADCSHT_2:  16 ADCCLK cycles
     * ADCON   :  ADC10 On/Enable
     *
     * ADCCTL1:
     * ADCSHP   : Sample-and-hold pulse-mode select
     *
     * ADCCTL2:
     * ADCRES_2 : 12 bit
     *
     * ADCMCTL0:
     * ADCINCH_1 :  A1 ADC input select; Vref=AVCC
     *
     * ADCIE: Enable ADC conv complete interrupt
     *
     */
    ADCCTL0 |= ADCSHT_2 | ADCON;

    ADCCTL1 |= ADCSHP;
    ADCCTL2 &= ~ADCRES;

    ADCCTL2 |= ADCRES_2;

    ADCMCTL0 |= ADCINCH_1;
    ADCIE |= ADCIE0;
}



unsigned int ADC_Result;

void main(void)
{
    /* Desabilita Watch dog */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* LED de depuração */
    P1DIR = BIT0;

    init_pga();
    init_adc12();

    while(1)
    {
        /* Trigger do ADC por software */
        ADCCTL0 |= ADCENC | ADCSC;
        __bis_SR_register(LPM0_bits | GIE);
        __no_operation();

        if (ADC_Result < 0x7FF)
            P1OUT &= ~BIT0;
        else
            P1OUT |= BIT0;

        __delay_cycles(5000);
    }
}


/* ISR do ADC12. Executada quando a conversão terminar */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
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
        /* Copia dados e acorda o main */
        ADC_Result = ADCMEM0;
        __bic_SR_register_on_exit(LPM0_bits);
        break;
    default:
        break;
    }
}
