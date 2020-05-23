/*
 * 06_main_adc_isr_dtc_timer.c
 *
 *  Created on: May 18, 2020
 *      Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo do conversor analógico digital.
 *      - Trigger do ADC por software.
 *      - 32 amostras são realizadas intecalando entre A0 e A1 (16 cada)
 *      - Usa-se Data Transfer Controller (DTC) para copiar dados
 *      do AD para memória sem interferência do CPU
 *
 *                  MSP430G2553
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *         -->  | P1.0 A0         |
 *         -->  | P1.1 A1         |
 *              |                 |
 *              |                 |
 */

#include <msp430.h>
#include <stdint.h>

#include "lib/bits.h"

/* Dados das amostras do ADC */
uint16_t adc_data[32] = {0};

/**
 * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
 *         Utililiza-se as calibrações internas gravadas na flash.
 *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c
 *         Configura ACLK para utilizar VLO = ~10KHz
 * @param  none
 *
 * @retval none
 */
void init_clock_system(){

    /* Configuração do MCLK em 1MHz */

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_1MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

/**
  * @brief  Configura conversor analógico digital (ADC10).
  *
  * @param  none
  *
  * @retval none
  */
void init_adc(){

    /* ADC10CTL0:
     * ADC10SHT_2: 16 x ADC10CLKs
     * ADC10ON   : ADC10 On/Enable
     * ADC10IE   : IRQ Enable
     * MSC       : ADC10 Multiple SampleConversion
     */
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;

    /* Quando em Multiple SampleConversion,
     * canais são amostrados do maior canal até A0
     * INCH_1: Canal 1
     *
     * CONSEQ_3: Repeat sequence of channels
     */
    ADC10CTL1 = INCH_1 + CONSEQ_3;

    /*  P1.0 P1.1 liga ADC aos pinos */
    ADC10AE0 = BIT0 | BIT1;

    /* Número de conversões em função do tamanho do vetor  */
    ADC10DTC1 = sizeof(adc_data)/(sizeof(uint16_t));
}




int main(void)
{
    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    init_clock_system();
    init_adc();

    /* Pino de saída para depuração */
    P1DIR = BIT6;

    for (;;)
    {
        /* Desabilitação do ADC */
        ADC10CTL0 &= ~ENC;
        /* Especa caso ADC ocupado */
        while (ADC10CTL1 & ADC10BUSY);

        /* Ponteiro para o DTC colocar os dados
         * ADC10SA deve receber o endereço antes do novo tigger
         */
        ADC10SA = (unsigned int)adc_data;

        /* Habilitação e trigger */
        ADC10CTL0 |= ENC + ADC10SC;
        __bis_SR_register(CPUOFF + GIE);

        /* Espaço para debugger */
        __no_operation();
        __no_operation();
    }
}

/* ISR do ADC10. Executada quando as conversões terminarem */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    /* Debug para verificar a ISR */
    CPL_BIT(P1OUT, BIT6);

    /* Acorda CPU quando todas as conversões são concluídas */
    __bic_SR_register_on_exit(CPUOFF);
}

