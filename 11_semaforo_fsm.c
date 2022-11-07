/*
 * fsm.c
 *
 *  Created on: Nov 7, 2022
 *      Author: Renan Augusto Starke
 *
 *      Um semáforo para exemplificar a implementação de FSM.
 *      WatchDog timer é utilizado como a base de tempo de 1 segundo
 *
 *
 *                  MSP430F5529LP
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *  BOTAO   --> | P1.1    P1.0    | --> LED VERMELHO
 *              |         P4.7    | --> LED VERDE
 *              |                 | --> Estado amarelo liga Vermelho e verde
 *
 */

#include <11_semaforo_fsm.h>
#include <msp430.h>
#include <stdint.h>

#include "./lib/gpio.h"
#include "./lib/bits.h"


/* Cabeçalho das funções dos estados */
void f_repouso(void);
void f_vermelho(void);
void f_amarelo(void);
void f_verde(void);

/* Definição da estrutura mantenedora do vetor de estados */
typedef struct {
    state_t myState;
    void (*func)(void);
}fsm_t;

/* Mapeamento entre estado e funções */
fsm_t myFSM[] = {
                 { ESTADO_REPOUSO, f_repouso },
                 { ESTADO_VERMELHO, f_vermelho },
                 { ESTADO_AMARELO, f_amarelo },
                 { ESTADO_VERDE, f_verde },
};

/* Estado atual  */
volatile state_t curr_state = ESTADO_REPOUSO;


/* Configuração de hardware */
void config_semaforo(){

    /* Botão como entrada e pull up */
    SET_BIT(PORT_REN(BOTAO_PORT),BOTAO_PIN);
    SET_BIT(PORT_OUT(BOTAO_PORT),BOTAO_PIN);

    /* Sinais como saída */
    SET_BIT(PORT_DIR(VERDE_PORT), VERDE_PIN);
    SET_BIT(PORT_DIR(AMARELO_PORT), AMARELO_PIN);
    SET_BIT(PORT_DIR(VERMELHO_PORT), VERMELHO_PIN);

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
    static uint8_t segundos = 0;

    segundos++;

    switch (curr_state){
    case ESTADO_REPOUSO:
        if (!TST_BIT(PORT_IN(BOTAO_PORT),BOTAO_PIN)) {
            segundos = 0;
            curr_state = ESTADO_VERMELHO;
        }

        break;

    case ESTADO_VERMELHO:

        if (segundos > 10) {
            segundos = 0 ;
            curr_state = ESTADO_VERDE;
        }

        break;

    case ESTADO_AMARELO:
        if (segundos > 5) {
            segundos = 0 ;
            curr_state = ESTADO_VERMELHO;
        }
        break;
    case ESTADO_VERDE:
        if (segundos > 15) {
            segundos = 0 ;
            curr_state = ESTADO_AMARELO;
        }
        break;

    default:
        break;
    }

    /* Acorda o main para alterar os LEDs */
    __bic_SR_register_on_exit(LPM0_bits);

}


void do_fsm(){
    (*myFSM[curr_state].func)();
}

void f_repouso(void){
    /* Desliga semáforo */
    CLR_BIT(PORT_OUT(VERDE_PORT), VERDE_PIN);
    CLR_BIT(PORT_OUT(AMARELO_PORT), AMARELO_PIN);
    CLR_BIT(PORT_OUT(VERMELHO_PORT), VERMELHO_PIN);
}

void f_vermelho(void){
    CLR_BIT(PORT_OUT(VERDE_PORT), VERDE_PIN);
    CLR_BIT(PORT_OUT(AMARELO_PORT), AMARELO_PIN);

    /* Liga Vermelho */
    SET_BIT(PORT_OUT(VERMELHO_PORT), VERMELHO_PIN);

}

void f_amarelo(void){
    /* Nessa aplicação, liga-se vermelho e verde como exemplo do amarelo */
    SET_BIT(PORT_OUT(VERDE_PORT), VERDE_PIN);
    SET_BIT(PORT_OUT(VERMELHO_PORT), VERMELHO_PIN);
    // SET_BIT(PORT_OUT(AMARELO_PORT), AMARELO_PIN);
}

void f_verde(void){
    CLR_BIT(PORT_OUT(AMARELO_PORT), AMARELO_PIN);
    CLR_BIT(PORT_OUT(VERMELHO_PORT), VERMELHO_PIN);

    /* Liga verde */
    SET_BIT(PORT_OUT(VERDE_PORT), VERDE_PIN);
}
