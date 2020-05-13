/*
 * control.c
 *
 *  Created on: May 13, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      Exemplo de aplicação:
 *
 *      - PWM em 50 Hz Timer A0
 *      - IRQ no Comparador A0: base de tempo da aplicação e couting deboucer
 *      - Máquina de estado (FSM) para o controle de um processo discreto.
 *
 */

/* Includes de sistema */
#include <msp430.h>
#include <stdint.h>

/* Includes do projeto */
#include "control.h"
#include "fsm.h"
#include "../../lib/gpio.h"
#include "../../lib/bits.h"

void config_timerA_as_pwm();

void control_init(){

    /* Configura botões */
   /* BUTTON_PORT totalmente como entrada */
   PORT_DIR(BUTTON_PORT) = LED | BIT6;
   /* Resistores de pull up */
   PORT_REN(BUTTON_PORT) = BUTTON;
   PORT_OUT(BUTTON_PORT) = BUTTON;

   config_timerA_as_pwm();
}


void do_control(){
    do_fsm();
}


void turn_servo_off(){
    TA0CCR1 = 0;
}

void move_servo(uint16_t angle){

    if (angle < 180){

        /* Angulo: [0,180]
         * timer ~= angulo * 10 + 1000 */
        TA0CCR1 = angle * 5 + 1000;
    }
}


void set_led(){
    SET_BIT(PORT_OUT(LED_PORT), LED);
}

void unset_led(){
    CLR_BIT(PORT_OUT(LED_PORT), LED);
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

    /* t = 1us * 2000 = 20ms = 50Hz */
    TA0CCR0 = 20000;
    TA0CCTL1 = OUTMOD_7;

    TA0CCTL0 = CCIE;

    /* 1ms = 1us * 1000 = 1ms */
    TA0CCR1 = 1000;

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
    * Timer +--------------|----|+----------->
    *       |              |    |
    *       |
    *
    *       |--------------+    |---------
    * Saída |              |    |
    *       +---------------+++++------------->
    */

    /* TASSEL_2 -> Timer A clock source select: 2 - SMCLK
     * MC_1     -> Timer A mode control: 1 - Up to CCR0
     * ID_3     ->  Timer A input divider: 3 - /8
     *
     * Configuração da fonte do clock do timer 1 */
    TA0CTL = TASSEL_2 | MC_1 | ID_3;


    /* TA0.1 é o P1.2 */
    P1DIR |= BIT2;
    /* Função alternativa de P1.2 -> TA0.1 */
    P1SEL |= BIT2;

}



/* ISR0 do Timer A: executado no evento de comparação  comparador 0 (TACCR0)
 *
 * Utilizado para toda a temporização da aplicação.
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
    static uint8_t button_state = 0;
    static uint16_t app_base_time = 0;
    static uint16_t seconds = 0;

    /* Debug: Piscar quando executar a ISR
    CPL_BIT(P1OUT, BIT6); */

    /* Se botão apertado: borda de descida */
    if (!TST_BIT(PORT_IN(BUTTON_PORT), BUTTON))  {
        /* Se contagem = 0, debounce terminado */
        if (!(--counter)) {

            button_state = 1;

            /* Acorda função main
            __bic_SR_register_on_exit(LPM0_bits); */
            CPL_BIT(PORT_OUT(LED_PORT), LED);
        }
    }
    else
        counter = BUTTON_SAMPLES;


    /* Transição de estados: 50 * 20ms: 1s
       * Base de tempo da apicação           */
    app_base_time++;

    if (app_base_time > 50) {

        app_base_time = 0;
        seconds++;

        switch (curr_state) {
            case STATE_A:
                if (button_state) {
                    button_state = 0;
                    curr_state = STATE_B;
                    seconds = 0;
                    __bic_SR_register_on_exit(LPM0_bits);
                }
                break;

            case STATE_B:
                if (seconds > 3) {
                    seconds = 0;
                    curr_state = STATE_C;
                    __bic_SR_register_on_exit(LPM0_bits);
                }
                break;

            case STATE_C:
                if (seconds > 4){
                    seconds = 0;
                    curr_state = STATE_D;
                    __bic_SR_register_on_exit(LPM0_bits);
                }
                break;

            case STATE_D:
                if (seconds > 5){
                    seconds = 0;
                    curr_state = STATE_A;
                    __bic_SR_register_on_exit(LPM0_bits);
                }
                break;

            default:
                break;
        }
    }
}
