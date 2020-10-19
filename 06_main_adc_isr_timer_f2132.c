/*
 * 06_main_adc_isr.c
 *
 *  Created on: May 18, 2020
 *      Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo do conversor analógico digital.
 *      - Trigger do ADC por software na ISR do timer
 *      - Trigger do ADC pelo timer não está funcionando no Proteus.
 *      - Temporização por um timer.
 *
 *      - Simulação em ./sim/analog/adc_lcd.pdsprj
 *
 *                  MSP430F2132
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *  LCD_RS <--  | P2.6    P2.0 A0 | <-- Potenciômetro
 *  LCD_E  <--  | P2.7    P2.1 A0 | <-- Potenciômetro
 *              |                 |
 * LCD_DATA <-- | P1.[0..3]       |
 */

#include <msp430.h>
/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>
#include <stdio.h>

#include "lib/gpio.h"
#include "lib/bits.h"
#include "displays/lcd.h"

#ifndef __MSP430F2132__
#error "Clock system not supported for this device"
#endif

volatile uint16_t adc_val_0 = 0;
volatile uint16_t adc_val_1 = 0;

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
//Configurar para usar o timer A
void timerA_init(){
    /* Timer A0:
     *
     * TASSEL_2 -> Clock de SMCLK.
     * MC_1 -> Contagem ate TCCR0.
     * ID_3 -> Prescaler = /8
     */
     TA0CTL = TASSEL_2 | MC_1 | ID_3 ;   //clock de 1 MHZ

     /* IRQ por comparação entre contagem e comparador 0 */
     TA0CCTL0 = CCIE;
     /* Valor de comparação é 40000 ciclos*/
     TA0CCR0 = 40000;

     P2DIR = BIT2;

}

void init_adc(){

    /* ADC10CTL0:
       * ADC10SHT_2:  16 x ADC10CLKs
       * ADC10ON   :  ADC10 On/Enable
       * ADC10IE   :  IRQ Enable
       */
    ADC10CTL0 |= SREF1 + ADC10ON + ADC10IE;

    /* ADC10CTL1:
        * INCH_0: Selects Channel 0
    */
    ADC10CTL1 = INCH_0;

    /*  P2.0 e P2.1 ADC option select */
    ADC10AE0 = BIT0 | BIT1;

    /* ADC10 Enable and start conversion */
    ADC10CTL0 |= ENC + ADC10SC;
}

int main (void)
{
    char string[16];

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Sistema de clock */
    init_clock_system();
    init_adc();
    timerA_init();

    /* Pino de debpuração */
    SET_BIT(P1DIR,BIT6);
    SET_BIT(P1DIR,BIT7);

    /* Inicializa hardare: veja lcd.h para
     * configurar pinos */
    lcd_init_4bits();
    /* Escreve string */

    lcd_send_data(LCD_LINE_1, LCD_CMD);
    lcd_write_string("MSP430");

    while (1){
        /* Desliga CPU até ADC terminar */
        __bis_SR_register(CPUOFF + GIE);

        /* Remove caracteres antigos do LCD */
        if (adc_val_0 < 1000)
            lcd_send_data(' ', LCD_DATA);
        if (adc_val_0 < 1000)
            lcd_send_data(' ', LCD_DATA);
        if (adc_val_0 < 10)
            lcd_send_data(' ', LCD_DATA);

        lcd_send_data(LCD_LINE_0, LCD_CMD);
        snprintf(string, 16, "%d %d", adc_val_0, adc_val_1);

        lcd_write_string(string);
    }
}

/* ISR0 do Timer A: executado no evento de comparação  comparador 0 (TACCR0) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    //CPL_BIT(P1OUT,BIT6);

    /* Trigger por software pelo timer.
     * Bug no Proteus
     */
    ADC10CTL0 |= ENC + ADC10SC;


    if ((adc_val_0 < 100 ) || (adc_val_1 < 100))
        SET_BIT(P1OUT,BIT6);
    else
        CLR_BIT(P1OUT,BIT6);


}

/* ISR do ADC10. Executada quando a conversão terminar */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    static uint8_t canal = 0;

    volatile uint16_t adc = ADC10MEM;


    /* Mux dos canais */
    switch (canal){
    case 0:
        adc_val_0 = adc_val_1;
        ADC10CTL1 = INCH_1;
        break;

    case 1:
        adc_val_1 = adc;
        ADC10CTL1 = INCH_0;
        break;
    }

    canal++;
    canal &= 0x1;

    /* Complementa P1.6 para ver velocidade do ADC */
    CPL_BIT(P1OUT,BIT7);

    __bic_SR_register_on_exit(CPUOFF);
}
