/*
 * 05_main_couting_deboucer.c
 *
 *  Created on: Mar 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de debouce de botão por pooling.
 *      Faz a verificação de botão periodicamente utilizando
 *      o comparador 0 do Timer A0
 *      Período do pooling deve ser maior que o tempo de
 *      instabilidade da chave.
 *
 *       .
 *      /|\                  +
 *       |                 +
 *       |               +
 *       |             +  <-- Comparador 0 (TACCR0  -> TIMER0_A0_VECTOR) - Debouncer do botão
 *       |           +
 *       |         +
 *       |       +
 *       |     +
 *       |   +
 *       | +
 *       +----------------------------------------->
 *
 *       - Comparadores podem ser configurados para qualquer valor
 *       entre 0 e 65535. IRQs devem ser habilitadas individuais
 *       nos respectivos registradores.
 *
 */

#ifndef __MSP430G2553__
    #error "Clock system not supported for this device"
#endif

/* System includes */
#include <msp430.h>

/* Project includes */
#include "lib/gpio.h"
#include "lib/bits.h"
#include "displays/simple_display_mux.h"

#define BUTTON_PORT P1
#define BUTTON BIT3

#define LED_PORT P1
#define LED BIT0

#define BUTTON_SAMPLES (12)

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

    /* Configuração do MCLK em 8MHz */

    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

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
     *
     * TASSEL_2 -> Clock de SMCLK.
     * MC_2 -> Contagem crescente.
     * ID_3 -> Prescaler = /8
     */
    TA0CTL = TASSEL_2 | MC_2 | ID_1;

    /* IRQ por comparação entre contagem e comparador 0 */
    TA0CCTL0 = CCIE;
    /* Valor de comparação é 50000 */
    TA0CCR0 = 20000;
}


int main(void)
{
    volatile uint16_t my_data = 0;

    /* Desliga watchdog imediatamente */
    WDTCTL = WDTPW | WDTHOLD;

    /* Configura botões */
    /* BUTTON_PORT totalmente como entrada */
    PORT_DIR(BUTTON_PORT) = LED | BIT6;
    /* Resistores de pull up */
    PORT_REN(BUTTON_PORT) = BUTTON;
    PORT_OUT(BUTTON_PORT) = BUTTON;

    /* Configurações de hardware */
    config_timerA_0();
    init_clock_system();

    /* Inicializa displays */

    /* Entra em modo de economia de energia com IRQs habilitadas */
    __bis_SR_register(LPM0_bits + GIE);

    while (1){



        /* Desligar CPU novamente */
       __bis_SR_register(LPM0_bits);
    }
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
    static uint8_t counter = BUTTON_SAMPLES;

    /* Debug: Piscar quando executar a ISR */
    CPL_BIT(P1OUT, BIT6);

    /* Se botão apertado: borda de descida */
    if (!TST_BIT(PORT_IN(BUTTON_PORT), BUTTON))  {
        /* Se contagem = 0, debounce terminado */
        if (!(--counter)) {

            /* Colocar aqui código da aplicação referente ao botão */

            /* Acorda função main
            __bic_SR_register_on_exit(LPM0_bits); */

            CPL_BIT(PORT_OUT(LED_PORT), LED);

        }
    }
    else
        counter = BUTTON_SAMPLES;
}
