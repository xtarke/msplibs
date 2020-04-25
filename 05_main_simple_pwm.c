/*
 * 05_main_simple_pwm.c
 *
 *  Created on: Apr 22, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de parcial de aplicação:
 *
 *      - Gera PWM com o temporizador A0.
 *
 *      - Use o MSP430F2132 para simular PWM no Proteus.
 *      -  http://e2e.ti.com/support/microcontrollers/msp430/f/166/p/702207/2588488
 */

#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

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
void config_timerA_as_pwm(){

    /* Estamos usando TA0CCR0 para contagem máxima
     * que permite controle preciso sobre o período
     * é possível usar o overflow */

    /* Configuração dos comparadores como PWM:
     * TA0CCR0: Timer0_A Capture/Compare 0: período do PWM
     * OUTMOD_7: PWM output mode: 7 - PWM reset/set - CCR1 reset/set
     * TA0CCR1 PWM duty cycle: TA0CCR1 / TA0CCR0 */

    TA0CCR0 = 512-1;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1 = 350;

    /*      .
    *      /|\                  +  Comparador 0: TA0CCR0 (máximo da contagem) -> período do PWM
    *       |                 + |
    *       |               +   |
    *       |------------- +  <--  Comparador 1: TA0CCR1 razão cíclica
    *       |           +  |    |
    *       |         +    |    |
    *       |       +      |    |
    *       |     +        |    |      +
    *       |   +          |    |    +
    *       | +            |    |  +
    *       +--------------|----|+----------->
    *       |              |    |
    *       |              |    |
    *       |--------------+    |---------
    *       |              |    |
    *       +---------------+++++------------->
    */

    /* TASSEL_2 ->  Timer A clock source select: 2 - SMCLK
     * MC_1     ->  Timer A mode control: 1 - Up to CCR0
     *
     * Configuração da fonte do clock do timer 1 */
    TA0CTL = TASSEL_2 + MC_1;
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
    config_timerA_as_pwm();

    /* TA0.1 é o P1.2 */
    P1DIR = BIT2;
    /* Função alternativa de P1.2 -> TA0.1 */
    P1SEL = BIT2;
    P1SEL2 = 0;

    while(1)
    {
        __bis_SR_register(LPM0_bits + GIE);

        /* Código de baixa prioridade da aplicação */
    }
}
