/*
 * 05_main_pooling_deboucer.c
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
 *       |             +  <-- Comparador 0 (TACCR0  -> TIMER0_A0_VECTOR) - Pooling do botão
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

#define BUTTON_0 BIT0
#define BUTTON_1 BIT1
#define BUTTON_2 BIT2
#define BUTTON_PORT P2

volatile union button_state_t {
    uint8_t all_bits;
    struct {
       uint8_t b0 : 1;
       uint8_t b1 : 1;
       uint8_t b2 : 1;
   };
} buttons;

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
    TA0CTL = TASSEL_2 | MC_2 | ID_3;

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
    PORT_DIR(BUTTON_PORT) = 0;
    /* Resistores de pull up */
    PORT_REN(BUTTON_PORT) = BUTTON_0 | BUTTON_1 | BUTTON_2;
    PORT_OUT(BUTTON_PORT) = BUTTON_0 | BUTTON_1 | BUTTON_2;
    /* Estado inicial do estado dos botões */
    buttons.all_bits = 0;

    /* Configurações de hardware */
    config_timerA_0();
    init_clock_system();

    /* Inicializa displays */
    watchdog_display_mux_init();

    P1DIR |= BIT6;
    CLR_BIT(P1OUT, BIT6);

    /* Entra em modo de economia de energia com IRQs habilitadas */
    __bis_SR_register(LPM0_bits + GIE);

    while (1){
        /* Atualiza valor do display em baixa prioridade */
        if (!buttons.b0)
            SET_BIT(my_data, BIT0);
        else if (!buttons.b1)
            SET_BIT(my_data, BIT4);
        else
            my_data = 0;

        /* Escreve novo valor */
        watchdog_display_mux_write(my_data);

        /* Desligar CPU novamente */
       __bis_SR_register(LPM0_bits);
    }
}


/* ISR0 do Timer A: executado no evento de comparação  comparador 0 (TACCR0)
 *
 * Utilizado para o debouncer por pooling: faz a verificação de botão
 * periodicamente. Período do pooling deve ser maior que o tempo de
 * instabilidade da chave.
 *
 * */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    /* Leitura do estado de todos os botões */
    buttons.all_bits = PORT_IN(BUTTON_PORT) & 0x07;

    /* Caso deseja ler apenas um, ou se botões estiver em portas diferentes
      buttons.b0 = !TST_BIT(PORT_IN(BUTTON_PORT), BUTTON_0);
    */

    /* Debug: Pisca P1.6 quando detectado algum botão */
  CPL_BIT(P1OUT, BIT6);


    /* Se algum botão foi apertado acorda o main */
    if (buttons.all_bits != 0x07){
               /* Acorda função main */
        __bic_SR_register_on_exit(LPM0_bits);
    }
}
