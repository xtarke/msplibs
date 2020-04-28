/*
 * 05_main_simple_pwm.c
 *
 *  Created on: Apr 22, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de parcial de aplicação:
 *
 *      - Gera multiplos PWMs com o temporizador A1.
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
 * @brief  Configura temporizador A1 com contagem up e down.
 *
 * @param  none
 *
 * @retval none
 */
void config_timerA_1_as_pwm(){

    /* Estamos usando TA1CCR0 para contagem máxima
     * que permite controle preciso sobre o período
     * é possível usar o overflow */

    /* Configuração dos comparadores como PWM:
     *
     * TA1CCR0: Timer1_A Capture/Compare 0: período do PWM
     *
     * OUTMOD_2: PWM output mode: 2 - PWM toggle/reset
     *
     * TA1CCR1 PWM duty cycle: TA1CCR1 / TA1CCR0 *
     * TA2CCR1 PWM duty cycle: TA2CCR1 / TA1CCR0 */

    TA1CCR0 = 512-1;

    TA1CCTL1 = OUTMOD_2;
    TA1CCTL2 = OUTMOD_2;

    TA1CCR1 = 256;
    TA1CCR2 = 100;

    /*      .
    *      /|\                  +                < -Comparador 0: (máximo da contagem) -> período do PWM
    *       |                 +   +
    *       |               +       +
    *       |-------------+---------- +          <--  Comparadores 1 e 2: razão cíclica
    *       |           +  |         | +
    *       |         +    |         |   +
    *       |       +      |         |     +
    *       |     +        |         |       +
    *       |   +          |         |         +
    *       | +            |         |           +
    * Timer +--------------|---- ----|-------------->
    *       |              |
    *       |
    *
    *       |--------------+         |--------------
    * Saída |              |         |
    *       +---------------++++++++++------------->
    */

    /* TASSEL_2 -> Timer A clock source select: 2 - SMCLK
     * MC_1     -> Timer A mode control: 1 - Up to CCR0
     * ID_3     ->  Timer A input divider: 3 - /8
     *
     * Configuração da fonte do clock do timer 1 */
    TA1CTL = TASSEL_2 | MC_3 | ID_3;
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
    config_timerA_1_as_pwm();

    /* Portas não utilizadas */
    P1DIR = 0xff;
    P1OUT = 0;
    P3DIR = 0xff;
    P3OUT = 0;


    /* Ligação físicas do timer nas portas */
    /* TA1.1 é o P2.1
     * TA1.1 é o P2.2
     *
     * P2.1 e P2.2 geram mesmo sinal PWM
     *
     * TA1.2 é o P2.4
     * TA1.2 é o P2.5
     *
     * P2.4 e P2.5 geram mesmo sinal PWM
     *
     * */
    P2DIR = BIT1 | BIT2 | BIT4 | BIT5;

    /* Função alternativa: ligação dos pinos no temporizador
     *
     * P2.1 e P2.2 -> TA1.1
     * P2.4 e P2.5 -> TA1.2 */
    P2SEL = BIT1 | BIT2 | BIT4 | BIT5;

    while(1)
    {
        __bis_SR_register(LPM0_bits + GIE);

        /* Código de baixa prioridade da aplicação */
    }
}
