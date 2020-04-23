/*
 * relogio.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

/* Includes de sistema */
#include <msp430.h>
#include <stdint.h>

/* Includes do projeto */
#include "gpio.h"
#include "bits.h"
#include "relogio.h"

#define BUTTON_SAMPLES 12

#define DEBUG

volatile struct clock_t {
    uint8_t segundos;
    uint8_t minutos;
    uint8_t horas;

    uint8_t em_ajuste;
} meu_relogio;


/**
  * @brief  Configura temporizador A.
  *
  * @param  none
  *
  * @retval none
  */
void config_timerA_0(){
    /* Timer A0:
     *
     * TASSEL_2 -> Clock de SMCLK.
     * MC_1 -> Contagem até TA0CCR0.
     * ID_1 -> Prescaler = /2       */
    TA0CTL = TASSEL_2 | MC_1 | ID_1;

    /* IRQ por comparação entre contagem e comparador 0 */
    TA0CCTL0 = CCIE;
    /* Valor de comparação: nessa comparação é o período do timer pois ele conta até CCR0 */
    TA0CCR0 = 500;
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
    WDTCTL = WDT_ADLY_250;
    /* Ativa IRQ do Watchdog */
    IE1 |= WDTIE;
}


void init_relogio(){
    meu_relogio.segundos = 0;
    meu_relogio.minutos = 0;
    meu_relogio.horas = 0;

    PORT_DIR(RELOGIO_BUTTONS_PORT) = ~(BUTTON_ADJ | BUTTON_HORA | BUTTON_MIN);  // 0011 1000  -> ~ -> 1100 0111

    /* Configura Timmers. Se timers forem compartilhados com outros modos, *
     * sugiro criar um módulo timers.c/.h
     */
    config_wd_as_timer();
    config_timerA_0();
}

uint8_t obter_segundos(){
    return meu_relogio.segundos;
}

uint8_t obter_minutos(){
    return meu_relogio.minutos;
}

uint8_t obter_horas(){
    return meu_relogio.horas;
}

uint8_t em_ajuste(){
    return meu_relogio.em_ajuste;
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
   CPL_BIT(P3OUT, BIT0);

   meu_relogio.segundos++;

   if (meu_relogio.segundos > 60){
       meu_relogio.segundos = 0;
       meu_relogio.minutos++;
   }

   if (meu_relogio.minutos > 60){
     meu_relogio.minutos = 0;
     meu_relogio.horas++;
   }

   if (meu_relogio.horas > 23)
       meu_relogio.horas = 0;


   /* Desativa suspensão do main */
   __bic_SR_register_on_exit(LPM0_bits);
}



/* ISR0 do Timer A: executado no evento de comparação  comparador 0 (TACCR0)
 *
 * Utilizado para o debouncer por amostragem: faz a verificação de botão
 * periodicamente.
 *
 * */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    static uint8_t counter_adj = BUTTON_SAMPLES;

    /* Debug: Piscar quando executar a ISR */
    CPL_BIT(P3OUT, BIT1);

    /* Se botão apertado: borda de descida */
    if (!TST_BIT(PORT_IN(RELOGIO_BUTTONS_PORT), BUTTON_ADJ))  {
        /* Se contagem = 0, debounce terminado */
        if (!(--counter_adj)) {

            /* Coloca relógio em modo ajuste */
            meu_relogio.em_ajuste ^= BIT0;

            /* Acorda função main */
            __bic_SR_register_on_exit(LPM0_bits);
        }
    }
    else
        counter_adj = BUTTON_SAMPLES;


    /* Criar código dos demais botões aqui */

}


